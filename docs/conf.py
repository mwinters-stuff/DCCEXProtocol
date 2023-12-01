# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

from sphinx.builders.html import StandaloneHTMLBuilder
import subprocess
import os

# Doxygen
subprocess.call('doxygen Doxyfile.in', shell=True)

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'DCCEXProtocol'
copyright = '2023 - Peter Akers, Peter Cole'
author = 'Peter Akers, Peter Cole'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
  # 'sphinx_sitemap',
  'sphinxcontrib.spelling',
  'sphinx_rtd_dark_mode',
  'breathe'
]

autosectionlabel_prefix_document = True

# Don't make dark mode the user default
default_dark_mode = False

spelling_lang='en_UK'
tokenizer_lang='en_UK'
spelling_word_list_filename = ['spelling_wordlist.txt']

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

highlight_language = 'c++'

numfig = True

numfig_format = {'figure': 'Figure %s'}

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']

html_logo = "./_static/images/logo.png"

html_favicon = "./_static/images/favicon.ico"

html_theme_options = {
    'style_nav_header_background': 'white',
    'logo_only': True,
    # Toc options
    'includehidden': True,
    # 'titles_only': False,
    'titles_only': True,
    'collapse_navigation': False,
    # 'navigation_depth': 3
     'navigation_depth': -1 
}
 
html_context = {
    'display_github': True,
    'github_user': 'DCC-EX',
    'github_repo': 'dccexprotocol',
    'github_version': 'sphinx/docs/',
}

html_css_files = [
    'css/dccex_theme.css',
    'css/sphinx_design_overrides.css',
]

# -- Breathe configuration -------------------------------------------------

breathe_projects = {
  "DCCEXProtocol": "_build/xml/"
}
breathe_default_project = "C++ Sphinx Doxygen Breathe"
breathe_default_members = ('members', 'undoc-members')
