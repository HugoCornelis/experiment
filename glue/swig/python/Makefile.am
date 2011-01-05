
SUBDIRS =

pythondir = $(prefix)/glue/swig/python/g3

nobase_python_HEADERS = \
	$(srcdir)/experiment/pulsegen_base.so \
	$(srcdir)/experiment/__cbi__.py \
	$(srcdir)/experiment/__init__.py 

nobase_python_DATA = \
	experiment/pulsegen_base.py \
	experiment/_pulsegen_base.so

EXTRA_DIST = \
	$(srcdir)/pulsegen.i \
	$(srcdir)/perfectclamp.i

BUILT_SOURCES = \
	$(srcdir)/pulsegen_wrap.c \
	$(srcdir)/perfectclamp_wrap.c \
	$(srcdir)/experiment/pulsegen_base.py \
	$(srcdir)/experiment/perfectclamp_wrap.py 

all: _pulsegen_base.so

_pulsegen_base.so: pulsegen_wrap.o
	$(PULSEGEN_SWIGLINKCOMMAND_PYTHON)

pulsegen_wrap.o: pulsegen_wrap.c
	gcc $(SWIGCOMPILELINKSFORPYTHON) -c $(srcdir)/pulsegen_wrap.c

pulsegen_wrap.c: 
	$(SWIG) -python -o $(srcdir)/pulsegen_wrap.c $(srcdir)/pulsegen.i

clean-local:
	rm -f $(srcdir)/*.o $(srcdir)/*.pyc $(srcdir)/*.so

CLEANFILES = \
	$(srcdir)/pulsegen_wrap.c \
	$(srcdir)/pulsegen_wrap.o \
	$(srcdir)/pulsegen_base.so \
	$(srcdir)/pulsegen_base.py