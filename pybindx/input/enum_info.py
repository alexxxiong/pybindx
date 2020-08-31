"""
Information for enum
"""

from pybindx.input import cpp_type_info


class CppEnumInfo(cpp_type_info.CppTypeInfo):

    """
    A container for enum types to be wrapped
    """

    def __init__(self, name, type_info_dict = None):

        super(CppEnumInfo, self).__init__(name, type_info_dict)

    @property
    def parent(self):
        return self.module_info