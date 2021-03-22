from pygccxml import declarations

from pybindx.writers import base_writer

class CppClassArgWrapperWriter(base_writer.CppBaseWrapperWriter):

    """
    Manage addition of method wrapper code
    """

    def __init__(self, class_info,
                 arg_decl,
                 class_decl,
                 wrapper_templates,
                 class_short_name=None):

        super(CppClassArgWrapperWriter, self).__init__(wrapper_templates)

        self.class_info = class_info
        self.arg_decl = arg_decl
        self.class_decl = class_decl
        self.class_short_name = class_short_name
        if self.class_short_name is None:
            self.class_short_name = self.class_decl

    def exclusion_critera(self):

        # Are any return types not wrappable
        exclusion_args = self.class_info.hierarchy_attribute_gather('calldef_excludes')

        # Don't include sub class (e.g. iterator) methods
        if self.arg_decl.parent != self.class_decl:
            return True

        # No private methods without over-rides
        if self.arg_decl.access_type == "private":
            return True

        return False

    def add_self(self, output_string):

        # Check for exclusions
        if self.exclusion_critera():
            return output_string

        # Which definition type

        # arg like char[24]
        base_has = hasattr(self.arg_decl.decl_type, "base")
        is_array = hasattr(self.arg_decl.decl_type, "size") and self.arg_decl.decl_type.size != 0
        # is_vector = True if '::std::vector' in self.arg_decl.decl_type.decl_string else False

        char_array_type = base_has and self.arg_decl.decl_type.base.decl_string == 'char'
        is_static = self.arg_decl.type_qualifiers.has_static
        is_readonly = declarations.is_const(self.arg_decl.decl_type)

        arg_base_type = ""
        if is_array:
            def_adorn = "_property"
            if base_has:
                arg_base_type = self.arg_decl.decl_type.base.decl_string
        # elif is_vector:
        #     def_adorn = "_property"
        #     arg_base_type = declarations.vector_traits.element_type(self.arg_decl.decl_type).decl_string
        elif is_static:
            if is_readonly:
                def_adorn = "_property_readonly"
            else:
                def_adorn = "_readwrite"
            def_adorn += "_static"
            # if declarations.is_arithmetic(self.arg_decl.decl_type):
            arg_base_type = self.arg_decl.decl_type.decl_string
        else:
            if is_readonly:
                def_adorn = "_readonly"
            else:
                def_adorn = "_readwrite"

        simple_arg_dict = {'def_adorn': def_adorn,
                           'arg_name': self.arg_decl.name,
                           'arg_base_type': arg_base_type,
                           'class_short_name': self.class_short_name}

        if is_array:
            if char_array_type:
                template = self.wrapper_templates["class_char_array_args"]
            else:
                template = self.wrapper_templates["class_normal_array_args"]
        # elif is_vector:
        #     template = self.wrapper_templates["class_normal_array_args"]
        elif is_static and is_readonly:
            template = self.wrapper_templates["class_static_readonly_args"]
        else:
            template = self.wrapper_templates["class_simple_args"]
        output_string += template.format(**simple_arg_dict)
        return output_string

    def add_override(self, output_string):

        if self.arg_decl.access_type == "private":
            return output_string

        arg_string = ""
        num_arg_types = len(self.arg_decl.argument_types)
        args = self.arg_decl.arguments
        for idx, eachArg in enumerate(self.arg_decl.argument_types):
            arg_string += eachArg.decl_string + " " + args[idx].name
            if idx < num_arg_types-1:
                arg_string += ", "

        const_adorn = ""
        if self.arg_decl.has_const:
            const_adorn = " const "

        overload_adorn = ""
        if self.arg_decl.virtuality == "pure virtual":
            overload_adorn = "_PURE"

        all_args_string = ""
        for idx, eachArg in enumerate(self.arg_decl.argument_types):
            all_args_string += ""*8 + args[idx].name
            if idx < num_arg_types-1:
                all_args_string += ", \n"

        return_string = self.arg_decl.return_type.decl_string
        override_dict = {'return_type': return_string,
                         'method_name': self.arg_decl.name,
                         'arg_string': arg_string,
                         'const_adorn': const_adorn,
                         'overload_adorn': overload_adorn,
                         'tidy_method_name': self.tidy_name(return_string),
                         'short_class_name': self.class_short_name,
                         'args_string': all_args_string,
                         }
        output_string += self.wrapper_templates["method_virtual_override"].format(**override_dict)
        return output_string
