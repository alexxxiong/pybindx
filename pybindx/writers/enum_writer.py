from pybindx.writers import base_writer


class CppEnumWrapperWriter(base_writer.CppBaseWrapperWriter):

    """
    Manage addition of free function wrapper code
    """

    def __init__(self, enum_info, wrapper_templates):

        super(CppEnumWrapperWriter, self).__init__(wrapper_templates)

        self.enum_info = enum_info
        self.wrapper_templates = wrapper_templates
        self.exclusion_args = []

    def add_self(self, output_string):
        arg_string = ""
        for value in self.enum_info.decl.values:
            arg_dict = {'arg_name': value[0],
                        'short_name': self.enum_info.decl.name}

            arg_string += self.wrapper_templates["enum_value_define"].format(**arg_dict)

        method_dict = {'short_name': self.enum_info.decl.name,
                       'values_defines': arg_string}
        output_string += self.wrapper_templates["enum_define"].format(**method_dict)
        return output_string
