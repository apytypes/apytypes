project(
    'aPy-types',
    'c', 'cpp',
    version : '0.0.1'
)

lib_src = [
    'src/apy_common.cc',
    'src/apy_fixed.cc',
    'src/apy_float.cc',
    'src/apy_float_contextmanager.cc',
    'src/ieee754_bit_fiddle.c'
]

# Python module incudes
python_installation = import('python').find_installation('python3')
pybind11_includes = run_command(
    python_installation, '-c', 'import pybind11; print(pybind11.get_include());',
    check : true
).stdout().strip()

# GNU compiler warnings
cpp_compiler_flags = [

    # Warnings
    '-Wpedantic', '-Wall', '-Wextra', '-Wcast-align', '-Wunused', '-Wundef',
    '-Wdouble-promotion', '-Wmisleading-indentation', '-Wduplicated-cond',
    '-Wduplicated-branches', '-Wlogical-op', '-fno-common', '-Wnull-dereference',
    '-Wold-style-cast', '-Wredundant-decls', '-Wundef', '-Wno-unused',
    '-Wsuggest-final-types', '-Wsuggest-override', '-Woverloaded-virtual',

    # Others
    '-std=c++17', '-O2', '-march=native', '-g3',
]

# https://mesonbuild.com/howtox.html#enable-threads
cc = meson.get_compiler('cpp')
gmp_dep = cc.find_library('gmp')

# The aPy-types library
apy_lib = library(
    'apy_lib',
    sources : lib_src,
    cpp_args : cpp_compiler_flags,
    dependencies : gmp_dep,
    install: true # Perhaps only works on Linux systems
)

# Python module
python_installation.extension_module(
    'apy_types', 
    sources : [
        'src/apy_types_wrapper.cc',
        'src/apy_fixed_wrapper.cc',
        'src/apy_float_wrapper.cc',
        'src/apy_float_context_wrapper.cc'
    ],
    include_directories : pybind11_includes,
    dependencies : python_installation.dependency(),
    link_with : apy_lib,
    cpp_args : '-std=c++17',
    install: true
)
