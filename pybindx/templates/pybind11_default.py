
class_cpp_header = """\
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
{includes}
#include "{class_short_name}.pybindx.hpp"
namespace py = pybind11;
typedef {class_full_name} {class_short_name};{smart_ptr_handle}
"""

classes_cpp_header = """\
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include "Parser.pybindx.hpp"
#include "wrapper_header_collection.h"
namespace py = pybind11;

"""

classes_cpp_context = """\
typedef {class_full_name} {class_short_name};{smart_ptr_handle}
"""

class_hpp_header = """\
#pragma once

namespace py = pybind11;
void register_{class_short_name}_class(py::module &m);

"""

classes_hpp_header = """\
#pragma once

namespace py = pybind11;

"""

classes_hpp_context = """\
void register_{class_short_name}_class(py::module &m);

"""

taf_proxy_call = """\
m.def("create_proxy_for_{class_short_name}", [](const string &obj)
{{
    taf::TC_AutoPtr<{class_short_name}> prx = nullptr;
    taf::Application::getCommunicator()->stringToProxy(obj, prx);

    return prx.get();
}});
"""

class_virtual_override_header = """\
class {class_short_name}_Overloads : public {class_short_name}{{
    public:
    using {class_short_name}::{class_base_name};
"""

class_virtual_override_footer = "}\n"

class_definition = """\
void register_{short_name}_class(py::module &m){{
py::class_<{short_name} {overrides_string} {ptr_support} {bases} >(m, "{short_alias}")
"""

method_virtual_override = """\
    {return_type} {method_name}({arg_string}){const_adorn} override {{
        PYBIND11_OVERLOAD{overload_adorn}(
            {tidy_method_name},
            {short_class_name},
            {method_name},
            {args_string});
    }}
"""

smart_pointer_holder = "PYBIND11_DECLARE_HOLDER_TYPE(T, {}<T>)"

free_function = """\
    m.def{def_adorn}("{function_name}", ({return_type}({self_ptr})({arg_signature}){const_adorn}) &{function_name}, {function_docs} {default_args});
"""

class_simple_args = """\
        .def{def_adorn}("{arg_name}", &{class_short_name}::{arg_name})
"""

class_static_readonly_args = """\
        .def{def_adorn}("{arg_name}", 
                        [](py::object /* self */) {{ return {class_short_name}::{arg_name}; }})
"""

class_static_readwrite_args = """\
        .def{def_adorn}("{arg_name}", 
                        [](py::object /* self */) {{ return {class_short_name}::{arg_name}; }}, 
                        [](py::object /* self */, const {arg_base_type}& arg) {{ {class_short_name}::{arg_name} = arg; }})
"""

class_char_array_args = """\
        .def{def_adorn}("{arg_name}", 
                        []({class_short_name} &self) {{ return self.{arg_name}; }}, 
                        []({class_short_name} &self, ::std::string& arg) {{ std::copy(arg.begin(), arg.end(), self.{arg_name}); }})
"""

class_normal_array_args = """\
        .def{def_adorn}("{arg_name}", 
                        []({class_short_name} &self) {{ py::list l; for (auto &item : self.{arg_name}) {{l.append(py::cast(item, py::return_value_policy::reference));}} return l;}}, 
                        []({class_short_name} &self, ::std::vector<{arg_base_type}>& arg) {{ std::copy(arg.begin(), arg.end(), self.{arg_name}); }})
"""

class_method = """\
        .def{def_adorn}(
            "{method_name_alias}", 
            ({return_type}({self_ptr})({arg_signature}){const_adorn}) &{class_short_name}::{method_name}, 
            {method_docs} {default_args} {call_policy})
"""

class_method_with_ref = """\
        .def{def_adorn}(
            "{method_name_alias}",
            [&]({class_short_name}& self{args})
            {{
                {ref_arg_defs}
                {return_arg} self.{method_name_alias}({origin_args});
                return {ref_args};
            }})
"""

enum_define = """\
    py::enum_<{short_name}>(m, "{short_name}", py::arithmetic())
{values_defines}
        .export_values();\n\n
"""
enum_value_define = """\
        .value("{arg_name}", {short_name}::{arg_name})
"""

class_method_argc_argv = """\
        .def("{method_name}", 
             []({class_short_name}& self, const std::vector<std::string>& args){{
                std::vector<char *> c_str;
                c_str.reserve(args.size());
                char *head = "{class_short_name}";
                c_str.push_back(head);
                for (auto &s : args)
                {{
                    c_str.push_back(const_cast<char *>(s.c_str()));
                }}

                {return_adorn} self.init(c_str.size(), c_str.data());
             }})
"""

class_repr_method = """\
        .def("__repr__", [](const {class_short_name} &arg) {{ return {method_name}(arg);}})
"""


template_collection = {'free_function': free_function,
                       'class_cpp_header': class_cpp_header,
                       'class_hpp_header': class_hpp_header,
                       'classes_cpp_header': classes_cpp_header,
                       'classes_hpp_header': classes_hpp_header,
                       'classes_cpp_context': classes_cpp_context,
                       'classes_hpp_context': classes_hpp_context,
                       'class_simple_args': class_simple_args,
                       'class_static_readonly_args': class_static_readonly_args,
                       'class_static_readwrite_args': class_static_readwrite_args,
                       'class_char_array_args': class_char_array_args,
                       'class_normal_array_args': class_normal_array_args,
                       'class_method': class_method,
                       'class_method_argc_argv': class_method_argc_argv,
                       'class_definition': class_definition,
                       'taf_proxy_call': taf_proxy_call,
                       'class_virtual_override_header': class_virtual_override_header,
                       'class_virtual_override_footer': class_virtual_override_footer,
                       'smart_pointer_holder': smart_pointer_holder,
                       'method_virtual_override': method_virtual_override,
                       'class_method_with_ref': class_method_with_ref,
                       'class_repr_method': class_repr_method,
                       'enum_define': enum_define,
                       'enum_value_define': enum_value_define,
                       }
