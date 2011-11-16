

SUBDIRS = 


AM_CFLAGS=-g3 -fPIC -Wmissing-prototypes -Wmissing-declarations -ffloat-store -I/usr/local/neurospaces/instrumentor -I$(srcdir)/.. -I$(builddir).. -include experiment/config.h -DPRE_PROTO_TRAVERSAL -D__NO_MATH_INLINES

#CFLAGS = -O0

pkglib_LIBRARIES = libeneurospaces.a

libeneurospaces_a_SOURCES = \
	experiment/neurospaces/pulsegen.c 

nobase_include_HEADERS = \
	experiment/neurospaces/pulsegen.h

