
# python_headers=`python-config --includes`
# python_cflags=`python-config --cflags`
# python_ldflags=`python-config --ldflags`
# python_libs=`python-config --libs`
# python_prefix=`python-config --prefix`


BUILT_SOURCES = \
	pulsegen_wrap.c \
	pulsegen_base.py 

all: _pulsegen_base.so

_pulsegen_base.so: pulsegen_wrap.o
	ld $(SWIGLDLINKSFORPYTHON) -o _pulsegen_base.so /usr/lib/bundle1.o pulsegen_wrap.o

pulsegen_wrap.o: pulsegen_wrap.c
	gcc $(SWIGCOMPILELINKSFORPYTHON) -c pulsegen_wrap.c

pulsegen_wrap.c: pulsegen.i
	$(SWIG) -python -o pulsegen_wrap.c pulsegen.i

clean-local:
	rm -f *.o *.pyc *.so

CLEANFILES = \
	pulsegen_wrap.c \
	pulsegen_wrap.o \
	pulsegen_base.so \
	pulsegen_base.py

