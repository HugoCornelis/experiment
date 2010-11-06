
# python_headers=`python-config --includes`
# python_cflags=`python-config --cflags`
# python_ldflags=`python-config --ldflags`
# python_libs=`python-config --libs`
# python_prefix=`python-config --prefix`


BUILT_SOURCES = \
	pulsegen_wrap.c \
	pulsegen_base.py 

dist-hook:
	cp $(srcdir)/pulsegen.i $(distdir)

all: _pulsegen_base.so

_pulsegen_base.so: pulsegen_wrap.o
	$(PULSEGEN_SWIGLINKCOMMAND_PYTHON)
#ld $(SWIGLDLINKSFORPYTHON) -o _pulsegen_base.so /usr/lib/bundle1.o pulsegen_wrap.o

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

