# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = "APyTypes"
copyright = "2023-2025, Oscar Gustafsson, Mikael Henriksson, Theodor Lindberg"
author = "Oscar Gustafsson, Mikael Henriksson, Theodor Lindberg"

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.autosummary",
    "sphinx.ext.intersphinx",
    "sphinx.ext.autosectionlabel",
    "sphinx_gallery.gen_gallery",
    "numpydoc",  # Needs to be loaded *after* autodoc.
    "sphinx_copybutton",
    "breathe",
    "m2r2",
]


templates_path = ["_templates"]
exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]


# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = "furo"
html_static_path = ["_static"]
html_logo = "_static/apytypeslogo.svg"
html_favicon = "_static/apytypes.ico"

html_theme_options = {
    # Add GitHub icon in footer
    "footer_icons": [
        {
            "name": "GitHub",
            "url": "https://github.com/apytypes/apytypes",
            "html": """
                <svg stroke="currentColor" fill="currentColor" stroke-width="0" viewBox="0 0 16 16">
                    <path fill-rule="evenodd" d="M8 0C3.58 0 0 3.58 0 8c0 3.54 2.29 6.53 5.47 7.59.4.07.55-.17.55-.38 0-.19-.01-.82-.01-1.49-2.01.37-2.53-.49-2.69-.94-.09-.23-.48-.94-.82-1.13-.28-.15-.68-.52-.01-.53.63-.01 1.08.58 1.23.82.72 1.21 1.87.87 2.33.66.07-.52.28-.87.51-1.07-1.78-.2-3.64-.89-3.64-3.95 0-.87.31-1.59.82-2.15-.08-.2-.36-1.02.08-2.12 0 0 .67-.21 2.2.82.64-.18 1.32-.27 2-.27.68 0 1.36.09 2 .27 1.53-1.04 2.2-.82 2.2-.82.44 1.1.16 1.92.08 2.12.51.56.82 1.27.82 2.15 0 3.07-1.87 3.75-3.65 3.95.29.25.54.73.54 1.48 0 1.07-.01 1.93-.01 2.2 0 .21.15.46.55.38A8.013 8.013 0 0 0 16 8c0-4.42-3.58-8-8-8z"></path>
                </svg>
            """,
            "class": "",
        },
    ],
    # Change link colors to "teal blue" from xkcd's color naming survey
    "light_css_variables": {
        "color-brand-primary": "#01889F",
        "color-brand-content": "#01889F",
        "color-brand-visited": "#7ebd01",
    },
    "dark_css_variables": {
        "color-brand-primary": "#01889F",
        "color-brand-content": "#01889F",
        "color-brand-visited": "#7ebd01",
    },
}

intersphinx_mapping = {
    "python": ("https://docs.python.org/3/", None),
    "matplotlib": ("https://matplotlib.org/", None),
    "numpy": ("https://numpy.org/doc/stable/", None),
    "nanobind": ("https://nanobind.readthedocs.io/en/latest/", None),
    "scipy": ("https://docs.scipy.org/doc/scipy/", None),
    "cocotb": ("https://docs.cocotb.org/en/stable/", None),
    "vunit": ("https://vunit.github.io/", None),
}

suppress_warnings = ["autosectionlabel.*"]

# Breathe Configuration
breathe_default_project = "APyTypes"

breathe_projects_source = {
    "APyTypes": (
        "../src/",
        [
            "apyarray.h",
            "apybuffer.h",
            "apycfixed.cc",
            "apycfixed.h",
            "apycfixed_util.h",
            "apycfixed_wrapper.cc",
            "apycfixedarray.cc",
            "apycfixedarray.h",
            "apycfixedarray_wrapper.cc",
            "apycfloat.cc",
            "apycfloat.h",
            "apycfloat_util.h",
            "apycfloat_wrapper.cc",
            "apycfloatarray.cc",
            "apycfloatarray.h",
            "apycfloatarray_wrapper.cc",
            "apyfixed.cc",
            "apyfixed.h",
            "apyfixed_util.h",
            "apyfixed_wrapper.cc",
            "apyfixedarray.cc",
            "apyfixedarray.h",
            "apyfixedarray_iterator.cc",
            "apyfixedarray_iterator.h",
            "apyfixedarray_wrapper.cc",
            "apyfloat.cc",
            "apyfloat.h",
            "apyfloat_util.h",
            "apyfloat_wrapper.cc",
            "apyfloatarray.cc",
            "apyfloatarray.h",
            "apyfloatarray_iterator.cc",
            "apyfloatarray_iterator.h",
            "apyfloatarray_wrapper.cc",
            "apytypes_common.cc",
            "apytypes_common.h",
            "apytypes_context_wrapper.cc",
            "apytypes_fwd.h",
            "apytypes_intrinsics.h",
            "apytypes_mp.cc",
            "apytypes_mp.h",
            "apytypes_scratch_vector.h",
            "apytypes_simd.cc",
            "apytypes_simd.h",
            "apytypes_util.h",
            "apytypes_wrapper.cc",
            "array_utils.h",
            "broadcast.h",
            "ieee754.h",
            "python_util.h",
        ],
    )
}

breathe_projects = {"APyTypes": "xml/"}

# -- Options for sphinx-gallery --
sphinx_gallery_conf = {
    "examples_dirs": "../examples",  # path to your example scripts
    "gallery_dirs": "examples",  # path to where to save gallery generated output
    "plot_gallery": "True",  # sphinx-gallery/913
    "filename_pattern": ".",
    "doc_module": ("apytypes",),
    "reference_url": {"apytypes": None},
    "image_scrapers": (
        #    qtgallery.qtscraper,
        "matplotlib",
    ),
    "reset_modules": (
        #    qtgallery.reset_qapp,
        "matplotlib",
    ),
}


# Do not skip __init__ of Python classes
def skip(app, what, name, obj, would_skip, options):
    if name == "__init__":
        return False
    return would_skip


def setup(app) -> None:
    app.connect("autodoc-skip-member", skip)


# Do not expect each method to have a separate page
numpydoc_class_members_toctree = False

# Do not add full paths
python_use_unqualified_type_names = True
