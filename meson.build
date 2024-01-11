project(
    'aPy-types',
    'c', 'cpp',
    default_options : ['cpp_std=c++20'],
    version : '0.0.1',
)

# Python module incudes
py3 = import('python').find_installation('python3', pure: false)
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
py3.install_sources(['lib/apy_types/__init__.py'], subdir: 'apy_types')

# Python module
py3.extension_module(
    '_apy_types',
    sources : files([
        'src/apy_common.cc',
        'src/apy_fixed.cc',
        'src/apy_fixed_wrapper.cc',
        'src/apy_float.cc',
        'src/apy_float_context_wrapper.cc',
        'src/apy_float_contextmanager.cc',
        'src/apy_float_wrapper.cc',
        'src/apy_types_wrapper.cc',
        'src/ieee754_bit_fiddle.c']),
    include_directories : pybind11_includes,
    dependencies : [py3_dep, gmp_dep, pybind11_dep],
    subdir: 'apy_types',
    install: true
)
