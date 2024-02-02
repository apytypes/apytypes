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
    "sphinx_gallery.gen_gallery",
    "numpydoc",  # Needs to be loaded *after* autodoc.
    "sphinx_copybutton",
    "breathe",
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
}

# Breathe Configuration
breathe_default_project = "APyTypes"

breathe_projects_source = {
    "APyTypes": (
        "../src/",
        [
            "apytypes_common.cc",
            "apyfixed.cc",
            "apyfloat.cc",
            "apyfloat_contextwrapper.cc",
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
