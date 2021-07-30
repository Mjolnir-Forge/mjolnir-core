# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))


import os
import subprocess

# -- Project information -----------------------------------------------------

project = "Mjolnir-Core"
copyright = "2021, Mjolnir-Forge"
author = "Mjolnir-Forge"


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    #    "breathe",
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ["_templates"]

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = "alabaster"

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ["_static"]

# Breathe Configuration
# breathe_default_project = "mjolnir_core"
# breathe_projects = dict(mjolnir_core="../build/xml/")


# read_the_docs_build = os.environ.get("READTHEDOCS", None) == "True"

# if read_the_docs_build:
wd = os.getcwd()
os.chdir("../..")

with open(".doxyfile", "a") as file:
    file.write("WARN_AS_ERROR = NO")

subprocess.run(["doxygen", "-v"])
r = subprocess.run(["doxygen", ".doxyfile"])
if r.returncode:
    raise Exception("The doxygen build failed")
r = subprocess.run(["ls", "docs/build/html"])
os.chdir(wd)

html_extra_path = ["../build/html"]

# sphinx-build source build_sphinx -b html
