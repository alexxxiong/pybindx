#!/usr/bin/env python

"""
This scipt automatically generates Python bindings using a rule based approach
"""

import os

from pybindx.writers import enum_writer
from pybindx.writers import free_function_writer
from pybindx.writers import class_writer
from pybindx.writers import classes_writer


class CppModuleWrapperWriter(object):

    def __init__(self, global_ns,
                 source_ns,
                 module_info,
                 wrapper_templates,
                 wrapper_root,
                 package_license=None):

        self.global_ns = global_ns
        self.source_ns = source_ns
        self.module_info = module_info
        self.wrapper_root = wrapper_root
        self.exposed_class_full_names = []
        self.wrapper_templates = wrapper_templates
        self.license = package_license

        self.exposed_class_full_names = []

    def generate_main_cpp(self):

        """
        Generate the main cpp for the module
        """

        # Generate the main cpp file
        module_name = self.module_info.name
        full_module_name = "_" + self.module_info.package_info.name + "_" + module_name

        cpp_string = ""
        cpp_string += '#include <pybind11/pybind11.h>\n'

        if self.module_info.package_info.common_include_file:
            cpp_string += '#include "wrapper_header_collection.hpp"\n'

        # Custom code
        if self.module_info.custom_generator is not None:
            cpp_string += self.module_info.custom_generator.get_module_pre_code()

        # Add includes
        if self.module_info.all_in_one == 1:
            cpp_string += '#include "Parser.pybindx.hpp"\n'
        else:
            for eachClass in self.module_info.class_info:
                for short_name in eachClass.get_short_names():
                    cpp_string += '#include "' + short_name + '.pybindx.hpp"\n'
        cpp_string += '\nnamespace py = pybind11;\n\n'
        for ns in self.module_info.namespace:
            cpp_string += 'using namespace ' + ns + ';\n'
        cpp_string += 'PYBIND11_MODULE(' + full_module_name + ', m)\n{\n'

        # Add enum
        for eachEnum in self.module_info.enum_info:
            writer = enum_writer.CppEnumWrapperWriter(eachEnum, self.wrapper_templates)
            cpp_string = writer.add_self(cpp_string)

        # Add free functions
        for eachFunction in self.module_info.free_function_info:
            if self.module_info.struct_repr is not None and eachFunction.name in self.module_info.struct_repr:
                # Print function jump off
                if len(eachFunction.decl.argument_types) == 1:
                    self.module_info.repr_mapping[eachFunction.decl.argument_types[0].base.base.decl_string] = eachFunction
                    continue
            writer = free_function_writer.CppFreeFunctionWrapperWriter(eachFunction,
                                                                       self.wrapper_templates)
            cpp_string = writer.add_self(cpp_string)

        # Add viable classes
        for eachClass in self.module_info.class_info:
            for short_name in eachClass.get_short_names():
                cpp_string += '    register_' + short_name + '_class(m);\n'

        # Add any custom code
        if self.module_info.custom_generator is not None:
            cpp_string += self.module_info.custom_generator.get_module_code()

        output_dir = self.wrapper_root + "/" + self.module_info.name + "/"
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
        main_cpp_file = open(output_dir + self.module_info.name + ".main.cpp", "w")
        main_cpp_file.write(cpp_string + '}\n')
        main_cpp_file.close()

    def get_class_writer(self, class_info):

        """
        Return the class writer, override for custom writers
        """

        this_class_writer = class_writer.CppClassWrapperWriter(class_info, self.wrapper_templates)
        return this_class_writer

    def get_classes_writer(self, class_info):

        """
        Return the class writer, override for custom writers
        """

        this_classes_writer = classes_writer.CppClassesWrapperWriter(class_info, self.wrapper_templates)
        return this_classes_writer

    def write(self):

        """
        Main method for writing the module
        """

        print('Generating Wrapper Code for: ' + self.module_info.name + ' Module.')

        self.generate_main_cpp()

        # Generate class files
        for eachClassInfo in self.module_info.class_info:
            self.exposed_class_full_names.extend(eachClassInfo.get_full_names())

        if self.module_info.all_in_one == 1:
            path = self.wrapper_root + "/" + self.module_info.name + "/" + "Parser"
            hpp_file = open(path + ".pybindx.hpp", "w")
            hpp_file.write(self.wrapper_templates["classes_hpp_header"])
            hpp_file.close()

            cpp_file = open(path + ".pybindx.cpp", "w")
            cpp_file.write(self.wrapper_templates["classes_cpp_header"])
            cpp_file.close()

            for eachClassInfo in self.module_info.class_info:
                print('Generating Wrapper Code for: ' + eachClassInfo.name + ' Class.')

                module_classes_writer = self.get_classes_writer(eachClassInfo)
                module_classes_writer.exposed_class_full_names = self.exposed_class_full_names
                for fullName in eachClassInfo.get_full_names():
                    class_decl = self.source_ns.class_(fullName.replace(" ", ""))
                    module_classes_writer.class_decls.append(class_decl)
                module_classes_writer.write(self.wrapper_root + "/" + self.module_info.name + "/")
        else:
            for eachClassInfo in self.module_info.class_info:

                print('Generating Wrapper Code for: ' + eachClassInfo.name + ' Class.')

                module_class_writer = self.get_class_writer(eachClassInfo)
                module_class_writer.exposed_class_full_names = self.exposed_class_full_names
                for fullName in eachClassInfo.get_full_names():
                    class_decl = self.source_ns.class_(fullName.replace(" ", ""))
                    module_class_writer.class_decls.append(class_decl)
                module_class_writer.write(self.wrapper_root + "/" + self.module_info.name + "/")
