#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
from setuptools import setup, Extension

setup(
        name = 'mytype',
        version = '0.0.1',

        package_data = {
                '': [ '**.txt', '**.md', '**.py', '**.h', '**.hpp', '**.c', '**.cpp' ],
            },

        ext_modules = [
            Extension(
                name = 'mytype',
                sources = [
                    'mytype.c',
                ],
                include_dirs = ['source'],
            )
        ],
    )
