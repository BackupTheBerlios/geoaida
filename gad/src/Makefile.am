####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = gad

## INCLUDES were found outside kdevelop specific part

gad_SOURCES = GadBool.cpp WildcardReplace.cpp MainGui.cpp main.cpp GadWorkspace.cpp GadText.cpp GadProcess.cpp GadObject.cpp GadModule.cpp GadIO.cpp GadGuiObjectTable.cpp GadGui.cpp GadEnum.cpp ClassDict.cpp 
SOURCES = GadBool.cpp WildcardReplace.cpp MainGui.cpp main.cpp GadWorkspace.cpp GadText.cpp GadProcess.cpp GadObject.cpp GadModule.cpp GadIO.cpp GadGuiObjectTable.cpp GadGui.cpp GadEnum.cpp ClassDict.cpp 
INTERFACES = 
TRANSLATIONS = 
KDE_OPTIONS = qtonly
gad_LDADD   = -lqtgui -lqtparser -lqt-mt -lXext -lX11 $(LIBSOCKET)


EXTRA_DIST = ClassDict.cpp ClassDict.h GadEnum.cpp GadEnum.h GadGui.cpp GadGui.h GadGuiObjectTable.cpp GadIO.cpp GadIO.h GadModule.cpp GadModule.h GadObject.cpp GadObject.h GadProcess.cpp GadProcess.h GadText.cpp GadText.h GadWorkspace.cpp GadWorkspace.h main.cpp MainGui.cpp MainGui.h WildcardReplace.cpp WildcardReplace.h gad.app GadBool.h GadBool.cpp 

install-data-local:
	$(mkinstalldirs) $(prefix)/share/data/application/
	$(INSTALL_DATA) $(srcdir)/gad.app $(prefix)/share/data/application/gad.app

uninstall-local:
	-rm -f $(prefix)/share/data/application/gad.app

####### kdevelop will overwrite this part!!! (end)############


# set the include path for X, qt and KDE
INCLUDES = -DPRGDIR=\"@prefix@/\" -DTMPDIR=\"@prefix@/tmp/\" $(all_includes)

gad_METASOURCES = AUTO

# the library search path. 
gad_LDFLAGS = $(all_libraries) 

# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(gad_METASOURCES)




