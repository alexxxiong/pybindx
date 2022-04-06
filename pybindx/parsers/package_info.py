import os
import imp
import ntpath
import yaml

from pybindx.input.package_info import PackageInfo
from pybindx.input.module_info import ModuleInfo
from pybindx.input.class_info import CppClassInfo
from pybindx.input.free_function_info import CppFreeFunctionInfo
from pybindx.input.enum_info import CppEnumInfo


class PackageInfoParser(object):

    def __init__(self, input_file, source_root):

        self.input_file = input_file
        self.raw_info = {}
        self.package_info = None
        self.source_root = source_root

    def subsititute_bool_string(self, option, input_dict, on_string="ON", off_string="OFF"):

        is_string = isinstance(input_dict[option], str)
        if is_string and input_dict[option].strip().upper() == off_string:
            input_dict[option] = False
        elif is_string and input_dict[option].strip().upper() == on_string:
            input_dict[option] = True

    def is_option_ALL(self, option, input_dict, check_string="ALL"):

        is_string = isinstance(input_dict[option], str)
        return is_string and input_dict[option].upper() == check_string

    def check_for_custom_generators(self, feature_info):

        # Replace source root if needed
        if feature_info.custom_generator is not None:
            path = feature_info.custom_generator.replace("UP_SOURCEROOT", self.source_root)
            path = os.path.realpath(path)
            print(feature_info.name, path)
            if os.path.isfile(path):
                module_name = ntpath.basename(path).split(".")[0]
                custom_module = imp.load_source(os.path.splitext(path)[0], path)
                feature_info.custom_generator = getattr(custom_module, module_name)()

    def parse(self):

        with open(self.input_file, 'r') as inpfile:
            data = inpfile.read()

        self.raw_info = yaml.safe_load(data)

        global_defaults = {'source_includes': [],
                           'smart_ptr_type': None,
                           'calldef_excludes': None,
                           'return_type_excludes': None,
                           'template_substitutions': [],
                           'pointer_call_policy': None,
                           'reference_call_policy': None,
                           'constructor_arg_type_excludes': None,
                           'excluded_methods': [],
                           'excluded_variables': [],
                           'custom_generator': None,
                           'prefix_code': []}

        # Parse package data
        package_defaults = {'name': 'up_package',
                            'common_include_file': True,
                            'source_hpp_patterns': ["*.h", "*.hpp"]}
        package_defaults.update(global_defaults)
        for eachEntry in package_defaults.keys():
            if eachEntry in self.raw_info:
                package_defaults[eachEntry] = self.raw_info[eachEntry]
        self.subsititute_bool_string('common_include_file', package_defaults)

        self.package_info = PackageInfo(package_defaults['name'], self.source_root, package_defaults)
        self.check_for_custom_generators(self.package_info)

        # Parse module data
        for eachModule in self.raw_info['modules']:
            module_defaults = {'name': 'up_module',
                               'all_in_one': 0,
                               'source_locations': None,
                               'namespace': [],
                               'classes': [],
                               'free_functions': [],
                               'variables': [],
                               'enums': [],
                               'global_function_mapping': None,
                               'class_alias': None,
                               'struct_repr': None,
                               'use_all_enums': True,
                               'use_all_classes': False,
                               'use_all_free_functions': False}
            module_defaults.update(global_defaults)

            for eachEntry in module_defaults.keys():
                if eachEntry in eachModule:
                    module_defaults[eachEntry] = eachModule[eachEntry]

            # Do enums
            enum_info_collection = []
            module_defaults['use_all_enums'] = self.is_option_ALL('enums', module_defaults)
            if not module_defaults['use_all_enums']:
                if module_defaults['enums'] is not None:
                    for eachEnum in module_defaults['enums']:
                        enum_defaults = {'name_override': None,
                                         'source_file': None}
                        enum_defaults.update(global_defaults)

                        class_info = CppEnumInfo(enum_defaults['name'], enum_defaults)
                        self.check_for_custom_generators(class_info)
                        enum_info_collection.append(class_info)

            # Do classes
            class_info_collection = []
            module_defaults['use_all_classes'] = self.is_option_ALL('classes', module_defaults)
            if not module_defaults['use_all_classes']:
                if module_defaults['classes'] is not None:
                    for eachClass in module_defaults['classes']:
                        class_defaults = {'name_override': None,
                                          'source_file': None}
                        class_defaults.update(global_defaults)

                        for eachEntry in class_defaults.keys():
                            if eachEntry in eachClass:
                                class_defaults[eachEntry] = eachClass[eachEntry]
                        class_info = CppClassInfo(eachClass['name'], class_defaults)
                        self.check_for_custom_generators(class_info)
                        class_info_collection.append(class_info)

            # Do class alias
            class_alias = dict()
            if module_defaults['class_alias'] is not None:
                for eachMapping in module_defaults['class_alias']:
                    if 'alias' in eachMapping:
                        alias = eachMapping['alias']
                        class_alias[alias[0]] = alias[1]

            # Do functions mapping
            function_mapping = dict()
            if module_defaults['global_function_mapping'] is not None:
                for eachMapping in module_defaults['global_function_mapping']:
                    if 'mapping' in eachMapping:
                        mapping = eachMapping['mapping']
                        function_mapping[mapping[0]] = mapping[1]

            # Do functions
            function_info_collection = []
            module_defaults['use_all_free_functions'] = self.is_option_ALL('free_functions',
                                                                           module_defaults)
            if not module_defaults['use_all_free_functions']:
                if module_defaults['free_functions'] is not None:
                    for _ in module_defaults['free_functions']:
                        ff_defaults = {'name_override': None,
                                       'source_file': None}
                        ff_defaults.update(global_defaults)
                        function_info = CppFreeFunctionInfo(ff_defaults['name'], ff_defaults)
                        function_info_collection.append(function_info)

            variable_collection = []
            use_all_variables = self.is_option_ALL('variables', module_defaults)
            if not use_all_variables:
                for _ in module_defaults['variables']:
                    variable_defaults = {'name_override': None,
                                         'source_file': None}
                    variable_defaults.update(global_defaults)
                    variable_info = CppFreeFunctionInfo(variable_defaults['name'], variable_defaults)
                    variable_collection.append(variable_info)

            module_info = ModuleInfo(module_defaults['name'], module_defaults)
            module_info.enum_info = enum_info_collection
            module_info.class_info = class_info_collection
            module_info.free_function_info = function_info_collection
            module_info.variable_info = variable_collection
            module_info.function_mapping = function_mapping
            module_info.class_alias = class_alias
            for class_info in module_info.class_info:
                class_info.module_info = module_info
            for free_function_info in module_info.free_function_info:
                free_function_info.module_info = module_info
            for variable_info in module_info.variable_info:
                variable_info.module_info = module_info
            self.package_info.module_info.append(module_info)
            module_info.package_info = self.package_info
            self.check_for_custom_generators(module_info)
