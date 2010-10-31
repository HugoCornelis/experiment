
python_headers=`python-config --includes`
python_cflags=`python-config --cflags`
python_ldflags=`python-config --ldflags`
python_libs=`python-config --libs`
python_prefix=`python-config --prefix`


BUILT_SOURCES = \
	pulsegen_wrap.c \
	pulsegen_base.py 

interface: pulsegen.i
	swig -python pulsegen.i


all: interface

CLEANFILES = \
	pulsegen_wrap.c \
	pulsegen_wrap.o \
	pulsegen_base.so \
	pulsegen_base.py

