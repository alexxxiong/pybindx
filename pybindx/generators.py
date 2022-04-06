import os
import logging
import fnmatch
import ntpath

from pybindx.input.info_helper import CppInfoHelper
from pybindx.input.package_info import PackageInfo
from pybindx.input.free_function_info import CppFreeFunctionInfo
from pybindx.input.class_info import CppClassInfo
from pybindx.input.enum_info import CppEnumInfo
from pybindx.parsers.package_info import PackageInfoParser
from pybindx.parsers.source_parser import CppSourceParser
from pybindx.writers.header_collection_writer import CppHeaderCollectionWriter
from pybindx.writers.module_writer import CppModuleWrapperWriter

import pybindx.templates.pybind11_default as wrapper_templates


class CppWrapperGenerator(object):

    def __init__(self, source_root,
                 source_includes=None,
                 wrapper_root=None,
                 castxml_binary='castxml',
                 package_info_path='package_info.yaml',
                 clang_binary='clang',
                 cflags='--std=c++11 -w',
                 source_header_files=None
                 ):

        logger = logging.getLogger()
        logger.setLevel(logging.INFO)

        self.source_root = source_root
        self.source_includes = source_includes
        self.wrapper_root = wrapper_root
        self.castxml_binary = castxml_binary
        self.clang_binary = clang_binary
        self.cflags = cflags
        self.package_info_path = package_info_path
        self.source_hpp_files = []
        if source_header_files is not None:
            self.source_hpp_files = source_header_files
        self.global_ns = None
        self.source_ns = None

        if self.wrapper_root is None:
            self.wrapper_root = self.source_root

        if self.source_includes is None:
            self.source_includes = [self.source_root]

        # collect all location path used
        self.source_dirs = {self.source_root}
        self.package_info = PackageInfo("up_package", self.source_root)

        # If we suspect that a valid info file has not been supplied
        # fall back to the default behaviour
        path_is_default = (self.package_info_path == 'package_info.yaml')
        file_exists = os.path.exists(self.package_info_path)
        if path_is_default and (not file_exists):
            logger.info('YAML package info file not found. Using default info.')
            self.package_info_path = None

    def collect_source_hpp_files(self):

        """
        Walk through the source root and add any files matching the provided patterns.
        Keep the wrapper root out of the search path to avoid pollution.
        """
        for pattern in self.package_info.source_hpp_patterns:
            for filename in fnmatch.filter(self.source_hpp_files, pattern):
                self.package_info.source_hpp_files.append(os.path.basename(filename))
                self.source_dirs.add(os.path.abspath(os.path.dirname(filename)))

        for root, _, filenames in os.walk(self.source_root, followlinks=True):
            for pattern in self.package_info.source_hpp_patterns:
                for filename in fnmatch.filter(filenames, pattern):
                    if "pybindx" not in filename:
                        self.package_info.source_hpp_files.append(os.path.join(root, filename))
        self.package_info.source_hpp_files = [path for path in self.package_info.source_hpp_files
                                              if self.wrapper_root not in path]

    def generate_header_collection(self):

        """
        Write the header collection to file
        """

        header_collection_writer = CppHeaderCollectionWriter(self.package_info,
                                                             self.wrapper_root)
        header_collection_writer.write()
        header_collection_path = self.wrapper_root + "/"
        header_collection_path += header_collection_writer.header_file_name

        return header_collection_path

    def parse_header_collection(self, header_collection_path):

        """
        Parse the header collection with pygccxml and Castxml
        to population the global and source namespaces
        """

        source_parser = CppSourceParser(self.source_dirs,
                                        header_collection_path,
                                        self.castxml_binary,
                                        self.source_includes,
                                        self.cflags,
                                        self.clang_binary)
        source_parser.parse()
        self.global_ns = source_parser.global_ns
        self.source_ns = source_parser.source_ns

    @staticmethod
    def get_wrapper_template():

        """
        Return the string templates for the wrappers
        """

        return wrapper_templates.template_collection

    def update_free_function_info(self):

        """
        Update the free function info parsed on pygccxml output
        """

        for eachModule in self.package_info.module_info:
            if eachModule.use_all_free_functions:
                free_functions = self.source_ns.free_functions(allow_empty=True)
                for eachFunction in free_functions:
                    if eachModule.is_decl_in_source_path(eachFunction):
                        function_info = CppFreeFunctionInfo(eachFunction.name)
                        function_info.module_info = eachModule
                        function_info.decl = eachFunction
                        eachModule.free_function_info.append(function_info)

            else:
                for eachFunction in eachModule.free_function_info:
                    functions = self.source_ns.free_functions(eachFunction.name,
                                                              allow_empty=True)
                    if len(functions) == 1:
                        eachFunction.decl = functions[0]

    def update_enum_info(self):

        """
        Update the enum info parsed on pygccxml output
        """

        for eachModule in self.package_info.module_info:
            if eachModule.use_all_enums:
                enumerations = self.source_ns.enumerations(allow_empty=True)
                for eachEnum in enumerations:
                    if eachModule.is_decl_in_source_path(eachEnum):
                        enum_info = CppEnumInfo(eachEnum.name)
                        enum_info.module_info = eachModule
                        enum_info.decl = eachEnum
                        eachModule.enum_info.append(enum_info)
            else:
                for eachEnum in eachModule.enum_info:
                    enumerations = self.source_ns.enumerations(eachEnum.name,
                                                               allow_empty=True)
                    if len(enumerations) == 1:
                        eachEnum.decl = enumerations[0]

    def update_class_info(self):

        """
        Update the class info parsed on pygccxml output
        """

        for eachModule in self.package_info.module_info:
            if eachModule.use_all_classes:
                classes = self.source_ns.classes(allow_empty=True)
                for eachClass in classes:
                    if eachModule.is_decl_in_source_path(eachClass):
                        if eachClass.name in eachModule.class_ignored:
                            continue
                        class_info = CppClassInfo(eachClass.name, {'excluded_variables': self.package_info.excluded_variables})
                        class_info.module_info = eachModule
                        class_info.decl = eachClass
                        eachModule.class_info.append(class_info)
            else:
                for eachClass in eachModule.class_info:
                    if eachClass.name in eachModule.class_ignored:
                        continue
                    classes = self.source_ns.classes(eachClass.name,
                                                     allow_empty=True)
                    if len(classes) == 1:
                        eachClass.decl = classes[0]

    def generate_wrapper(self):

        """
        Main method for wrapper generation
        """

        # If there is an input file, parse it
        if self.package_info_path is not None:
            info_parser = PackageInfoParser(self.package_info_path,
                                            self.source_root)
            info_parser.parse()
            self.package_info = info_parser.package_info
        else:
            pass

        # Generate a header collection
        self.collect_source_hpp_files()

        # Attempt to assign source paths to each class, assuming the containing 
        # file name is the class name
        for eachModule in self.package_info.module_info:
            for eachClass in eachModule.class_info:
                for eachPath in self.package_info.source_hpp_files:
                    base = ntpath.basename(eachPath)
                    if eachClass.name == base.split('.')[0]:
                        eachClass.source_file_full_path = eachPath
                        if eachClass.source_file is None:
                            eachClass.source_file = base

        # Attempt to automatically generate template args for each class
        for eachModule in self.package_info.module_info:
            info_generator = CppInfoHelper(eachModule)
            for eachClass in eachModule.class_info:
                info_generator.expand_templates(eachClass, "class")

        # Generate the header collection
        header_collection_path = self.generate_header_collection()

        # Parse the header collection
        self.parse_header_collection(header_collection_path)

        # Update the Class and Free Function Info from the parsed code
        self.update_class_info()
        self.update_free_function_info()
        self.update_enum_info()

        # Write the modules
        for eachModule in self.package_info.module_info:
            module_writer = CppModuleWrapperWriter(self.global_ns,
                                                   self.source_ns,
                                                   eachModule,
                                                   self.get_wrapper_template(),
                                                   self.wrapper_root)
            module_writer.write()
