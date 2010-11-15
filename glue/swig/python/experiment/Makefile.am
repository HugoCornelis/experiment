

BUILT_SOURCES = \
	perfectclamp_wrap.c \
	perfectclamp_base.py 

dist-hook:
	cp $(srcdir)/perfectclamp.i $(distdir)

all: _perfectclamp_base.so



#---------------------------------------------------------
#-- Rules for building the perfectclamp swig binding -----

_perfectclamp_base.so: perfectclamp_wrap.o
	$(PERFECTCLAMP_SWIGLINKCOMMAND_PYTHON)

perfectclamp_wrap.o: perfectclamp_wrap.c
	gcc $(SWIGCOMPILELINKSFORPYTHON) -c $(srcdir)/perfectclamp_wrap.c

perfectclamp_wrap.c: 
	$(SWIG) -python -o $(srcdir)/perfectclamp_wrap.c $(srcdir)/perfectclamp.i


#------------ End rules for perfectclamp swig ------------
#---------------------------------------------------------

clean-local:
	rm -f $(srcdir)/*.o $(srcdir)/*.pyc $(srcdir)/*.so

CLEANFILES = \
	$(srcdir)/perfectclamp_wrap.c \
	$(srcdir)/perfectclamp_wrap.o \
	$(srcdir)/perfectclamp_base.so \
	$(srcdir)/perfectclamp_base.py

