project(
    'apytypes',
    'cpp', 'c',
    default_options : ['cpp_std=c++17', 'c_std=c17'],
    version : '0.0.1.dev0',
)

# Python module incudes
py3 = import('python').find_installation('python3', pure: false)
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
# Version handling
fs = import('fs')
if fs.exists('_version.py')
    py3.install_sources('_version.py', subdir: 'apytypes')
else
  cfg = configuration_data()
  cfg.set_quoted('VCS_TAG', meson.project_version())
  configure_file(
    input: 'lib/apytypes/_version.py.in', output: '_version.py',
    configuration: cfg,
    install: true,
    install_tag: 'python-runtime',
    install_dir: py3.get_install_dir() / 'apytypes')
endif

# fmt
fmt = subproject('fmt')
fmt_dep = fmt.get_variable('fmt_header_only_dep')

# Python module
py3.extension_module(
    '_apytypes',
    sources : files([
        'src/apytypes_common.cc',
        'src/apyfixed.cc',
        'src/apyfixed_wrapper.cc',
        'src/apyfixedarray.cc',
        'src/apyfixedarray_wrapper.cc',
        'src/apyfloat.cc',
        'src/apyfloat_context_wrapper.cc',
        'src/apyfloat_contextmanager.cc',
        'src/apyfloat_wrapper.cc',
        'src/apytypes_wrapper.cc',
        'extern/mini-gmp/mini-gmp.c'
    ]),
    dependencies : [py3_dep, pybind11_dep, fmt_dep],
    subdir: 'apytypes',
    install: true
)
