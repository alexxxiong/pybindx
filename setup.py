#!/usr/bin/env python
# -*- coding: utf-8 -*-

from setuptools import setup, find_packages
from distutils.util import get_platform

print(get_platform())

setup(
    name='pybindx',  # 包名称
    version='1.1.18',
    keywords='autogenerate, pybind11',  # 关键词描述
    description='a simple auto generate for pybind11, could worked with only head file',   # 简略描述
    author='Alex Xiong',  # 作者信息
    packages=find_packages(),
    install_requires=[
        "pygccxml",
    ]
)
