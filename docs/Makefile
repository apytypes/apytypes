# Minimal makefile for Sphinx documentation
#

# You can set these variables from the command line, and also
# from the environment for the first two.
SPHINXOPTS    ?= --keep-going
SPHINXBUILD   ?= python3 -msphinx
SPHINXPROJ    = apytypes
SOURCEDIR     = .
BUILDDIR      = _build

# Put it first so that "make" without argument is like "make help".
help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: help Makefile

# workaround because sphinx does not completely clean up (#11139)
clean:
	@$(SPHINXBUILD) -M clean "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)
	rm -rf "$(SOURCEDIR)/examples"

show:
	@python3 -c "import webbrowser; webbrowser.open_new_tab('file://$(shell pwd)/_build/html/index.html')"

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile
	doxygen
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)
	sed -i 's/apytypes._apytypes.//g' $(BUILDDIR)/html/api/*.html
