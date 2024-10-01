project(
    'apytypes',
    'cpp', 'c',
    default_options : ['cpp_std=c++17', 'c_std=c17', 'b_lto=true'],
    version : run_command(find_program('python3'), '-m', 'setuptools_scm', check: true).stdout().strip(),
)

# Specify the compile-time limb size. Exactly one of `32`, `64`, or `NATIVE`
add_project_arguments(
    '-DCOMPILER_LIMB_SIZE=NATIVE',
    language: ['cpp', 'c'],
)

# Disable nanobind leak warnings on release builds
if get_option('buildtype') == 'release'
  add_project_arguments('-DNANOBIND_LEAK_WARNINGS=0', language: ['cpp'])
else
  add_project_arguments('-DNANOBIND_LEAK_WARNINGS=1', language: ['cpp'])
endif

# Python module includes
py3 = import('python').find_installation('python3', pure: false)
py3_dep = py3.dependency()

# Nanobind dependency
nanobind_dep = dependency('nanobind')

# Google Highway SIMD dynamic dispatching library
# Thanks germandiagogomez for the Meson/CMake fix for Google Highway
cmake = import('cmake')
cmake_opts = cmake.subproject_options()
if get_option('buildtype') == 'release'
    cmake_opts.add_cmake_defines({
        'CMAKE_BUILD_TYPE'    : 'Release',
    })
endif

# No need to build extras for Google Highway
cmake_opts.add_cmake_defines({
    'HWY_ENABLE_TESTS'    : 'OFF',
    'HWY_ENABLE_CONTRIB'  : 'OFF',
    'HWY_ENABLE_INSTALL'  : 'OFF',
    'HWY_ENABLE_EXAMPLES' : 'OFF',
})

hwy = cmake.subproject('highway', options: cmake_opts)
hwy_dep = hwy.dependency('hwy', include_type: 'system')

py3.install_sources(
    [
        'lib/apytypes/__init__.py',
        'lib/apytypes/__init__.pyi',
        'lib/apytypes/_apytypes.pyi',
        'lib/apytypes/_array_functions.py',
        'lib/apytypes/_array_functions.pyi',
        'lib/apytypes/_typing.py',
        'lib/apytypes/py.typed',
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
        install_dir: py3.get_install_dir() / 'apytypes'
    )
endif

# fmt
fmt = subproject('fmt')
fmt_dep = fmt.get_variable('fmt_header_only_dep')

# Python module
py3.extension_module(
    '_apytypes',
    sources : files([
        'extern/mini-gmp/mini-gmp.c',
        'src/apyfixed.cc',
        'src/apyfixed_util.cc',
        'src/apyfixed_wrapper.cc',
        'src/apyfixedarray.cc',
        'src/apyfixedarray_iterator.cc',
        'src/apyfixedarray_wrapper.cc',
        'src/apyfloat.cc',
        'src/apyfloat_util.cc',
        'src/apyfloat_wrapper.cc',
        'src/apyfloatarray.cc',
        'src/apyfloatarray_iterator.cc',
        'src/apyfloatarray_wrapper.cc',
        'src/apytypes_common.cc',
        'src/apytypes_context_wrapper.cc',
        'src/apytypes_wrapper.cc',
        'src/apytypes_simd.cc',
    ]),
    dependencies : [py3_dep, nanobind_dep, hwy_dep, fmt_dep],
    subdir: 'apytypes',
    install: true,
)
