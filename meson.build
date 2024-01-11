project(
    'aPy-types',
    'c', 'cpp',
    version : '0.0.1',
    default_options : ['cpp_std=c++17'],
)

lib_src = [
    'src/apy_common.cc',
    'src/apy_fixed.cc',
    'src/apy_float.cc',
    'src/apy_float_contextmanager.cc',
    'src/ieee754_bit_fiddle.c'
]

# Python module incudes
py3 = import('python').find_installation('python3')
pybind11_includes = run_command(
    py3, '-c', 'import pybind11; print(pybind11.get_include());',
    check : true
).stdout().strip()

# GNU compiler warnings
cpp_compiler_flags = [

    # Warnings
    '-Wpedantic', '-Wall', '-Wextra',

    # Others
    '-O2', '-march=native', '-g3',
]

# https://mesonbuild.com/howtox.html#enable-threads
cc = meson.get_compiler('cpp')
gmp_dep = cc.find_library('gmp')

py3_dep = py3.dependency()

pybind11_dep = dependency('pybind11', version: '>=2.6')

# Python module
py3.extension_module(
    'apy_types', 
    sources : [
        'src/apy_types_wrapper.cc',
        'src/apy_fixed_wrapper.cc',
        'src/apy_float_wrapper.cc',
        'src/apy_float_context_wrapper.cc'
    ],
    include_directories : pybind11_includes,
    dependencies : [py3.dependency(), gmp_dep, pybind11_dep],
    install: true
)
