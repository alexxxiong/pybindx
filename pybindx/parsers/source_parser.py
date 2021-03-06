#!/usr/bin/env python

"""
Parse the single header file using CastXML and pygccxml
"""

import os
from pygccxml import parser, declarations


class CppSourceParser:

    def __init__(self, source_root, wrapper_header_collection,
                 castxml_binary, source_includes, cflags='--std=c++11 -w', clang_binary='clang'):
        self.source_root = source_root
        self.wrapper_header_collection = wrapper_header_collection
        self.castxml_binary = castxml_binary
        self.source_includes = source_includes
        self.global_ns = None
        self.source_ns = None
        self.cflags = cflags
        self.clang_binary = clang_binary

    def parse(self):
        xml_generator_config = parser.xml_generator_configuration_t(xml_generator_path=self.castxml_binary,
                                                                    xml_generator='castxml',
                                                                    compiler_path=self.clang_binary,
                                                                    ignore_gccxml_output=True,
                                                                    cflags=self.cflags,
                                                                    include_paths=self.source_includes)

        print("INFO: Parsing Code")
        decls = parser.parse([self.wrapper_header_collection], xml_generator_config,
                             compilation_mode=parser.COMPILATION_MODE.ALL_AT_ONCE)

        # Get access to the global namespace
        self.global_ns = declarations.get_global_namespace(decls)

        # Clean decls to only include those for which file locations exist
        print("INFO: Cleaning Decls")
        query = declarations.custom_matcher_t(lambda decl: decl.location is not None)
        decls_loc_not_none = self.global_ns.decls(function=query)

        # Identify decls in our source tree
        def check_loc(loc):
            for source_root in self.source_root:
                if os.path.realpath(source_root) in loc or (source_root in loc):
                    return True

            if "wrapper_header_collection" in loc:
                return True
            return False

        source_decls = [decl for decl in decls_loc_not_none if check_loc(decl.location.file_name)]
        self.source_ns = declarations.namespace_t("source", source_decls)

        print("INFO: Optimizing Decls")
        self.source_ns.init_optimizer()
