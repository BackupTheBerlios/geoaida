####### kdevelop will overwrite this part!!! (begin)##########


EXTRA_DIST = guide.pdf guide.tex 

install-data-local:
	$(mkinstalldirs) $(prefix)/doc/gda/
	$(INSTALL_DATA) $(srcdir)/guide.pdf $(prefix)/doc/gda/guide.pdf

uninstall-local:
	-rm -f $(prefix)/doc/gda/guide.pdf

####### kdevelop will overwrite this part!!! (end)############
guide.pdf: guide.tex
	  pdflatex guide.tex
	  
