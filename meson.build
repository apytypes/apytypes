project('aPy-types', 'cpp', version : '0.0.1')

lib_src = ['src/cpp/apy_fixed_point.cpp']

# Python module incudes
python_installation = import('python').find_installation('python3')
pybind11_includes = run_command(python_installation, '-c', 'import pybind11; print(pybind11.get_include());', check : true).stdout().strip()

# GNU compiler warnings
GNU_W_Flags = [
    '-Wpedantic', '-Wall', '-Wextra', '-Wcast-align', '-Wunused', '-Wundef', '-Wconversion',
    '-Wdouble-promotion', '-Wsign-conversion', '-Wmisleading-indentation', '-Wduplicated-cond',
    '-Wduplicated-branches', '-Wlogical-op', '-fno-common', '-Wnull-dereference', '-Wuseless-cast',
    '-Wold-style-cast', '-Wredundant-decls', '-Wundef', '-Wno-unused', '-Wsuggest-final-types',
    '-Wsuggest-override', '-Woverloaded-virtual', '-std=c++17'
]

# aPy Types library
apy_lib = library('lib',
        sources : lib_src,
        cpp_args : GNU_W_Flags        
)

# Executable that can be for miscellaneous tests
executable('out',
    sources : ['src/cpp/main.cpp'],
    include_directories : ['src/cpp/'],
    link_with : apy_lib
)

# Python module
python_installation.extension_module('apy_types', 
    sources : 'src/cpp/pybind_fixed.cpp',
    include_directories : pybind11_includes,
    dependencies : python_installation.dependency(),
    link_with : apy_lib
)