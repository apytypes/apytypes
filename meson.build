project(
    'aPy-types',
    'cpp',
    default_options : ['cpp_std=c++17'],
    version : '0.0.1',
)

# Python module incudes
py3 = import('python').find_installation('python3', pure: false)
pybind11_includes = run_command(
    py3, '-c', 'import os; import pybind11; print(os.path.relpath(pybind11.get_include()));',
    check : true
).stdout().strip()

# https://mesonbuild.com/howtox.html#enable-threads
cc = meson.get_compiler('cpp')
#gmp_dep = cc.find_library('gmp')
py3_dep = py3.dependency()
pybind11_dep = dependency('pybind11', version: '>=2.6')
py3.install_sources(
  [
    'lib/apytypes/__init__.py',
    'lib/apytypes/__init__.pyi',
    'lib/apytypes/_apytypes.pyi',
  ],
  subdir: 'apytypes'
)

# Python module
py3.extension_module(
    '_apytypes',
    sources : files([
        'src/apytypes_common.cc',
        'src/apyfixed.cc',
        'src/apyfixed_wrapper.cc',
        'src/apyfloat.cc',
        'src/apyfloat_context_wrapper.cc',
        'src/apyfloat_contextmanager.cc',
        'src/apyfloat_wrapper.cc',
        'src/apytypes_wrapper.cc',
        'src/mini-gmp/mini-gmp.cc'
    ]),
    include_directories : pybind11_includes,
    #dependencies : [py3_dep, gmp_dep, pybind11_dep],
    dependencies : [py3_dep, pybind11_dep],
    subdir: 'apytypes',
    install: true
)
