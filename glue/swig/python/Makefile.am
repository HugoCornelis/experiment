
SUBDIRS =

pythondir = $(prefix)/glue/swig/python/g3

nobase_python_HEADERS = \
	$(srcdir)/experiment/_pulsegen_base.so \
	$(srcdir)/experiment/__cbi__.py \
	$(srcdir)/experiment/__init__.py 

nobase_python_DATA = \
	experiment/pulsegen_base.py \
	experiment/_pulsegen_base.so \
	experiment/perfectclamp_base.py \
	experiment/_perfectclamp_base.so 

EXTRA_DIST = \
	$(srcdir)/pulsegen.i \
	$(srcdir)/perfectclamp.i

BUILT_SOURCES = \
	$(srcdir)/pulsegen_wrap.c \
	$(srcdir)/experiment/pulsegen_base.py \
	$(srcdir)/perfectclamp_wrap.c \
	$(srcdir)/experiment/perfectclamp_base.py

all: _pulsegen_base.so _perfectclamp_base.so


#-----------------------------------------------------------
#-------- Rules for building the pulsegen swig binding -----
_pulsegen_base.so: pulsegen_wrap.o
	$(PULSEGEN_SWIGLINKCOMMAND_PYTHON)

pulsegen_wrap.o: pulsegen_wrap.c
	gcc $(SWIGCOMPILELINKSFORPYTHON) -c $(srcdir)/pulsegen_wrap.c

pulsegen_wrap.c: 
	$(SWIG) -I$(top_builddir) -python -outdir $(srcdir)/experiment -o $(srcdir)/pulsegen_wrap.c $(srcdir)/pulsegen.i

#------------------ End rules for pulsegen swig ------------
#-----------------------------------------------------------


#-----------------------------------------------------------
#---- Rules for building the perfectclamp swig binding -----
_perfectclamp_base.so: perfectclamp_wrap.o
#	$(PERFECTCLAMP_SWIGLINKCOMMAND_PYTHON)

perfectclamp_wrap.o: perfectclamp_wrap.c
	gcc $(SWIGCOMPILELINKSFORPYTHON) -c $(srcdir)/perfectclamp_wrap.c

perfectclamp_wrap.c: 
	$(SWIG) -I$(top_builddir) -python -outdir $(srcdir)/experiment -o $(srcdir)/perfectclamp_wrap.c $(srcdir)/perfectclamp.i

#-------------- End rules for perfectclamp swig ------------
#-----------------------------------------------------------

clean-local:
	rm -f $(srcdir)/*.o $(srcdir)/*.pyc $(srcdir)/*.so

CLEANFILES = \
	$(srcdir)/pulsegen_wrap.c \
	$(srcdir)/pulsegen_wrap.o \
	$(srcdir)/experiment/_pulsegen_base.so \
	$(srcdir)/experiment/pulsegen_base.py \
	$(srcdir)/perfectclamp_wrap.c \
	$(srcdir)/perfectclamp_wrap.o \
	$(srcdir)/experiment/perfectclamp_base.so \
	$(srcdir)/experiment/perfectclamp_base.py 
