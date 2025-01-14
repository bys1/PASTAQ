import os
import re
import sys
import sysconfig
import platform
import subprocess

from distutils.version import LooseVersion
from setuptools import setup, find_packages, Extension
from setuptools.command.build_ext import build_ext


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError(
                "CMake must be installed to build the following extensions: " +
                ", ".join(e.name for e in self.extensions))

        if platform.system() == "Windows":
            cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)',
                                                   out.decode()).group(1))
            if cmake_version < '3.1.0':
                raise RuntimeError("CMake >= 3.1.0 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(
            os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                      '-DPYTHON_EXECUTABLE=' + sys.executable]

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]

        if platform.system() == "Windows":
            cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(
                cfg.upper(),
                extdir)]
            if sys.maxsize > 2**32:
                cmake_args += ['-DCMAKE_GENERATOR_PLATFORM=x64']
                #cmake_args += ['G', 'Visual Studio 16 2019', '-A', 'x64']
            subprocess.check_call(['dir'])
            subprocess.check_call(['dir', '..'])
            build_args += ['--', '/m']
        else:
            cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
            build_args += ['--', '-j2']

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(
            env.get('CXXFLAGS', ''),
            self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args,
                              cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args,
                              cwd=self.build_temp)
        print()  # Add an empty line for cleaner output

with open("README.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()

setup(
    name='pastaq',
    version='0.9.0',
    author='Alejandro Sanchez Brotons',
    author_email='a.sanchez.brotons@rug.nl',
    description='Pipelines And Systems for Threshold Avoiding Quantification (PASTAQ): Pre-processing tools for LC-MS/MS data',
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/bys1/PASTAQ",
    packages=find_packages('src/python-bindings'),
    package_dir={'': 'src/python-bindings'},
    ext_modules=[CMakeExtension('pastaq/pastaq_cpp')],
    cmdclass=dict(build_ext=CMakeBuild),
    install_requires=[
        'numpy',
        'pandas',
        'seaborn',
        'matplotlib',
    ],
    zip_safe=False,
)
