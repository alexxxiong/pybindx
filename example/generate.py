#!/usr/bin/env python

"""
This script generates the wrapper code
"""

import os
from argparse import ArgumentParser
from glob import glob
from pybindx import CppWrapperGenerator


def generate_wrapper_code(source_root, wrapper_root, castxml_binary,
                          package_info_path, includes, clang_binary, cflags):
    generator = CppWrapperGenerator(source_root,
                                    includes,
                                    wrapper_root,
                                    castxml_binary,
                                    package_info_path,
                                    clang_binary,
                                    cflags)
    generator.generate_wrapper()


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument('--source_root', '-s', type=str,
                        help='Root of the source directory.',
                        default=os.getcwd())
    parser.add_argument('--wrapper_root', '-w', type=str,
                        help='Root of the wrapper directory.',
                        default=os.getcwd())
    parser.add_argument('--castxml_binary', '-c', type=str,
                        help='Path to the castxml binary.',
                        default='castxml')
    parser.add_argument('--clang_binary', '-cl', type=str,
                        help='Path to the clang binary.',
                        default='clang')
    parser.add_argument('--cflags', '-cf', type=str,
                        help='Cxx compile flags.',
                        default='--std=c++11 -w')
    parser.add_argument('--package_info', '-p', type=str,
                        help='Path to the package info file.',
                        default=os.getcwd() + "/package_info.yaml")
    parser.add_argument('--includes', '-i', type=str, nargs='+',
                        help='Path to the includes directory.',
                        default=os.getcwd())

    args = parser.parse_args()

    generate_wrapper_code(args.source_root, args.wrapper_root, args.castxml_binary,
                          args.package_info, args.includes, args.clang_binary, args.cflags)

