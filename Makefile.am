##
## Experiment : a compartmental solver that implements efficient Crank-Nicolson
## integration for neuronal models.
##

##############################################################################
##'
##' Experiment : testbed C implementation
##'
##' Copyright (C) 2006-2008 Hugo Cornelis
##'
##' functional ideas ..	Hugo Cornelis, hugo.cornelis@gmail.com
##'
##' coding ............	Hugo Cornelis, hugo.cornelis@gmail.com
##'
##############################################################################


SUBDIRS = \
	. \
	@EXPERIMENT_INTEGRATORS@ \
	glue \
	tests

dist_bin_SCRIPTS = bin/experiment

AM_CFLAGS=-g3 -fPIC -include experiment/config.h -Wmissing-prototypes -Wmissing-declarations -ffloat-store -D__NO_MATH_INLINES

#CFLAGS = -O0

lib_LIBRARIES = libexperiment.a

libexperiment_a_SOURCES = \
	output.c \
	perfectclamp.c \
	pulsegen.c \
	vclamp.c

nobase_include_HEADERS = \
	experiment/output.h \
	experiment/perfectclamp.h \
	experiment/pulsegen.h \
	experiment/vclamp.h

CLEANFILES = \
	$(top_srcdir)/docs/doxygen.tag

experimenttestdir=$(prefix)/experiment
nobase_dist_experimenttest_DATA=@TEST_SPECIFICATION_FILES@

RELEASE_MAJOR=`release-extract --major`
RELEASE_MINOR=`release-extract --minor`
RELEASE_MICRO=`release-extract --micro`

show-release-labels:
	echo "Release information (major-minor-micro): $(RELEASE_MAJOR)-$(RELEASE_MINOR)-$(RELEASE_MICRO)"

dist-keywords:
	release-expand experiment $(RELEASE_MAJOR) $(RELEASE_MINOR) $(RELEASE_MICRO) $(RELEASE_MAJOR).$(RELEASE_MINOR) hugo.cornelis@gmail.com --verbose
#	release-expand experiment des 10 0 des-10 hugo.cornelis@gmail.com --verbose

# the tests are copied overhere.
#
# The variable EXTRA_DIST could also be used here.

am__tar = ${AMTAR} chf - --format=posix "$$tardir"

clean-local:
	$(RM) -rf $(top_srcdir)/docs/html
	$(RM) -rf $(top_srcdir)/docs/latex
	$(RM) -rf $(top_srcdir)/docs/rtf
	$(RM) -rf $(top_srcdir)/docs/experiment
	$(RM) -rf $(top_srcdir)/*_BUILD
	$(RM) -f $(top_srcdir)/build_*.log
	$(RM) -f $(top_srcdir)/*.dsc
	$(RM) -f $(top_srcdir)/*.changes
	$(RM) -f $(top_srcdir)/*.deb
	$(RM) -f $(top_srcdir)/experiment-*.tar.gz

dist-hook: 
	if test -n "$$NEUROSPACES_RELEASE" ; then cd $(srcdir) && if test -n "`mtn ls changed `" ; then false ; else true ; fi ; else true ; fi
	find $(distdir) -name _Inline -exec chmod u+w -R \{\} \;
	$(RM) -fr `find $(distdir) -name _Inline`
	cp $(srcdir)/license.txt $(distdir)
	cp -R $(srcdir)/docs $(distdir)
	rm -rf $(distdir)/docs/html
	rm -rf $(distdir)/docs/latex
	rm -rf $(distdir)/docs/man
	rm -rf $(distdir)/docs/html
	rm -rf $(distdir)/docs/rtf
	$(srcdir)/install-sh -d $(distdir)/tests
	$(srcdir)/install-sh -d $(distdir)/tests/html
#	$(srcdir)/install-sh -d $(distdir)/tests/html/algorithms
#	$(srcdir)/install-sh -d $(distdir)/tests/html/code
#	$(srcdir)/install-sh -d $(distdir)/tests/html/specifications
#	$(srcdir)/install-sh -d $(distdir)/tests/html/specifications/algorithms
#	$(srcdir)/install-sh -d $(distdir)/tests/html/specifications/code
	cp -pR $(srcdir)/tests/command_extractor $(distdir)/tests
	cp -pR $(srcdir)/tests/command_extractor.ptkdb $(distdir)/tests
	cp -pR $(srcdir)/tests/tests_2_html $(distdir)/tests
	cp -pR $(srcdir)/tests/tests_2_html.ptkdb $(distdir)/tests
	cp -pR $(srcdir)/tests/tests.config $(distdir)/tests
	cp -pR $(srcdir)/tests.config $(distdir)
	cp -pR $(srcdir)/tests/code/tests.config $(distdir)/tests/code/tests.config
#	cp -pR $(srcdir)/hierarchy $(distdir)
#	rm -fr $(distdir)/hierarchy/output
#	( cd $(distdir)/tests && chmod -R u+w . && ./tests_2_html --introduction introduction.html )

#! this one breaks the distcheck, because automake makes dist dirs read-only
#	cd $(distdir)/tests && ./tests_2_html --introduction introduction.html

test: check

html-upload-prepare:
	( cd tests && ./tests_2_html --introduction introduction.html )
	rm -fr html/htdocs/neurospaces_project/experiment/
	mkdir -p html/htdocs/neurospaces_project/experiment
	mkdir -p html/htdocs/neurospaces_project/experiment/html_source
	tar cfh - tests/html | ( cd html/htdocs/neurospaces_project/experiment && tar vxf - )
# remove swig generated large files.
#rm -fr ~/HTML/local_home/local_home/hugo/neurospaces_project/experiment/source/c/snapshots/0/glue
#( cd ~/HTML/local_home/local_home/hugo && tar cfh - Xrefs/experiment experiment neurospaces_project/experiment ) | ( cd html/htdocs/neurospaces_project/experiment/html_source && tar vxf - )

html-upload: html-upload-prepare
	scp -pr html/htdocs/* hcornelis,neurospaces@web.sourceforge.net:/home/groups/n/ne/neurospaces/htdocs/

doxygen: experiment.dxgen
	$(DOXYGEN) experiment.dxgen

docs-level1:

docs-level2:

docs-level3: html-upload-prepare
	echo "Converted the regression tests to HTML"

docs-level4:

docs-level5:

docs-level6: doxygen
	echo "Doxygenized source code"

docs-level7:

docs: docs-level1 docs-level2 docs-level3 docs-level4 docs-level5 docs-level6 docs-level7



TMP_DIR = tmp


# Create a deb package
pkg-deb: dist
	nspkg-deb $(PACKAGE) $(VERSION)

# create an rpm package
pkg-rpm: dist
	nspkg-rpm $(PACKAGE) $(VERSION)


pkg-osx: 
	nspkg-osx 

