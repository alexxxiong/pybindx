#!/usr/bin/env python

"""
This script generates the wrapper code
"""

import os
import platform
import sys
import subprocess
import pkg_resources
from argparse import ArgumentParser

required = {'pybindx', 'pygccxml', 'pyyaml'}

for req in required:
    python = sys.executable
    if req == 'yaml':
        os.system(python + ' -m pip install --upgrade pyyaml')
        continue

    os.system(python + ' -m pip install --upgrade ' + req)

from pybindx import CppWrapperGenerator


def is_windows_system():
    return 'Windows' in platform.system()


def is_linux_system():
    return 'Linux' in platform.system()


def is_macos_system():
    return 'Darwin' in platform.system()


def generate_wrapper_code(source_root, source_header_files, wrapper_root, castxml_binary,
                          package_info_path, includes, clang_binary, cflags):
    generator = CppWrapperGenerator(source_root,
                                    includes,
                                    wrapper_root,
                                    castxml_binary,
                                    package_info_path,
                                    clang_binary,
                                    cflags,
                                    source_header_files)
    generator.generate_wrapper()


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument('--source_root', '-s', type=str,
                        help='Root of the source directory.',
                        default=os.getcwd())
    parser.add_argument('--source_header_files', '-sh', type=str, nargs='+',
                        help='Root of the source files.',
                        default=None)
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

    if is_windows_system():
        args.cflags = '-std=c++14 -w'
    else:
        if is_linux_system():
            os.system('chmod +x ' + args.clang_binary)

        os.system('chmod +x ' + args.castxml_binary)
        args.cflags = '--std=c++11 -w'

    generate_wrapper_code(args.source_root, args.source_header_files, args.wrapper_root, args.castxml_binary, args.package_info,
                          args.includes, args.clang_binary, args.cflags)
