# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = "APyTypes"
copyright = "2024, Oscar Gustafsson, Mikael Henriksson, Theodor Lindberg"
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


intersphinx_mapping = {
    "python": ("https://docs.python.org/3/", None),
    "numpy": ("https://numpy.org/doc/stable/", None),
    "nanobind": ("https://nanobind.readthedocs.io/en/latest/", None),
}

# Breathe Configuration
breathe_default_project = "APyTypes"

breathe_projects_source = {
    "APyTypes": (
        "../src/",
        [
            "apytypes_common.h",
            "apytypes_common.cc",
            "apytypes_simd.h",
            "apytypes_simd.cc",
            "apyfixed.h",
            "apyfixed.cc",
            "apyfixedarray.cc",
            "apyfixedarray.h",
            "apyfixedarray_iterator.cc",
            "apyfixedarray_iterator.h",
            "apyfixed_util.cc",
            "apyfixed_util.h",
            "apyfixed_wrapper.cc",
            "apyfloat.cc",
            "apyfloat.h",
            "apyfloatarray.cc",
            "apyfloatarray.h",
            "apyfloatarray_iterator.cc",
            "apyfloatarray_iterator.h",
            "apyfloat_util.cc",
            "apyfloat_util.h",
            "apyfloat_wrapper.cc",
            "apytypes_context_wrapper.cc",
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


def setup(app):
    app.connect("autodoc-skip-member", skip)


# Do not expect each method to have a separate page
numpydoc_class_members_toctree = False

# Do not add full paths
python_use_unqualified_type_names = True
