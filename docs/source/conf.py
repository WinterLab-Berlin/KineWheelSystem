# Configuration file for the Sphinx documentation builder.

# -- Project information

project = 'KWA-DLC'
copyright = '2023, BA'
author = 'BA'

release = '0.1'
version = '0.1.0'

# -- General configuration

extensions = [
    'myst_parser',
    'sphinx.ext.duration',
    'sphinx.ext.doctest',
    'sphinx.ext.autodoc',
    'sphinx.ext.autosummary',
    'sphinx.ext.intersphinx',
]

# Enable internal document links to headers (# -> h1, ## -> h2, ...)
# Without it, header links won't work in markdown.
# C.f. https://myst-parser.readthedocs.io/en/latest/syntax/optional.html#auto-generated-header-anchors
myst_heading_anchors = 6

intersphinx_mapping = {
    'python': ('https://docs.python.org/3/', None),
    'sphinx': ('https://www.sphinx-doc.org/en/master/', None),
}
intersphinx_disabled_domains = ['std']

templates_path = ['_templates']

# -- Options for HTML output

html_theme = 'sphinx_rtd_theme'

# These folders are copied to the documentation's HTML output
html_static_path = ['_static']

# Override list-style in RTD theme (theme.css) (to make bullet points visible)
# URL: https://docs.readthedocs.io/en/stable/guides/adding-custom-css.html?highlight=theme#overriding-or-replacing-a-theme-s-stylesheet
html_style = 'css/override.css'

# -- Options for EPUB output
epub_show_urls = 'footnote'
