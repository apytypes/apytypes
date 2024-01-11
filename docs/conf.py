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
            "apy_common.cc",
            "apy_fixed.cc",
            "apy_float.cc",
            "apy_float_contextwrapper.cc",
        ],
    )
}
