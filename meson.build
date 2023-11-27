project('aPy-types', 'cpp', version : '0.0.1')

lib_src = ['src/apy_fixed.cc', 'src/apy_float.cc']

# Python module incudes
python_installation = import('python').find_installation('python3')
pybind11_includes = run_command(python_installation, '-c', 'import pybind11; print(pybind11.get_include());', check : true).stdout().strip()

# GNU compiler warnings
GNU_W_Flags = [
    '-Wpedantic', '-Wall', '-Wextra', '-Wcast-align', '-Wunused', '-Wundef', '-Wconversion',
    '-Wdouble-promotion', '-Wsign-conversion', '-Wmisleading-indentation', '-Wduplicated-cond',
    '-Wduplicated-branches', '-Wlogical-op', '-fno-common', '-Wnull-dereference', '-Wuseless-cast',
    '-Wold-style-cast', '-Wredundant-decls', '-Wundef', '-Wno-unused', '-Wsuggest-final-types',
    '-Wsuggest-override', '-Woverloaded-virtual', '-std=c++17', '-g3'
]

# https://mesonbuild.com/howtox.html#enable-threads
cc = meson.get_compiler('cpp')
gmp_dep = cc.find_library('gmp')

# aPy Types library
apy_lib = library('lib',
        sources : lib_src,
        cpp_args : GNU_W_Flags,
        dependencies : gmp_dep     
)

# Python module
python_installation.extension_module('apy_types', 
    sources : ['src/apy_types_wrapper.cc', 'src/apy_fixed_wrapper.cc', 'src/apy_float_wrapper.cc'],
    include_directories : pybind11_includes,
    dependencies : python_installation.dependency(),
    link_with : apy_lib
)