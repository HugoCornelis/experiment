
setup: setup.py
	$(PYTHON) setup.py build
	$(PYTHON) setup.py build_py

install-exec-hook:
	$(PYTHON) setup.py install

uninstall-hook:
	$(PYTHON) uninstall.py

all: setup

# SUBDIRS =

# pythondir = $(prefix)/glue/swig/python/

# nobase_python_HEADERS = \
# 	$(srcdir)/neurospaces/experiment/__cbi__.py \
# 	$(srcdir)/neurospaces/experiment/__init__.py \
# 	$(srcdir)/neurospaces/experiment/pulsegen.py \
# 	$(srcdir)/neurospaces/experiment/perfectclamp.py \
# 	$(srcdir)/neurospaces/experiment/output.py 

# nobase_python_DATA = \
# 	neurospaces/experiment/pulsegen_base.py \
# 	neurospaces/experiment/_pulsegen_base.so \
# 	neurospaces/experiment/perfectclamp_base.py \
# 	neurospaces/experiment/_perfectclamp_base.so \
# 	neurospaces/experiment/output_base.py \
# 	neurospaces/experiment/_output_base.so 

# EXTRA_DIST = \
# 	$(srcdir)/pulsegen.i \
# 	$(srcdir)/perfectclamp.i \
# 	$(srcdir)/output.i \
# 	$(srcdir)/live_output.i 

# BUILT_SOURCES = \
# 	$(srcdir)/pulsegen_wrap.c \
# 	$(srcdir)/neurospaces/experiment/pulsegen_base.py \
# 	$(srcdir)/perfectclamp_wrap.c \
# 	$(srcdir)/neurospaces/experiment/perfectclamp_base.py \
# 	$(srcdir)/output_wrap.c \
# 	$(srcdir)/neurospaces/experiment/output_base.py

# all: _pulsegen_base.so _perfectclamp_base.so _output_base.so

# #-----------------------------------------------------------
# #-------- Rules for building the pulsegen swig binding -----
# _pulsegen_base.so: pulsegen_wrap.o
# 	$(PULSEGEN_SWIGLINKCOMMAND_PYTHON)

# pulsegen_wrap.o: pulsegen_wrap.c
# 	gcc $(SWIGCOMPILELINKSFORPYTHON) -c $(srcdir)/pulsegen_wrap.c

# pulsegen_wrap.c: 
# 	$(SWIG) -I$(top_builddir) -python -outdir $(srcdir)/neurospaces/experiment -o $(srcdir)/pulsegen_wrap.c $(srcdir)/pulsegen.i

# #------------------ End rules for pulsegen swig ------------
# #-----------------------------------------------------------


# #-----------------------------------------------------------
# #---- Rules for building the perfectclamp swig binding -----
# _perfectclamp_base.so: perfectclamp_wrap.o
# 	$(PERFECTCLAMP_SWIGLINKCOMMAND_PYTHON)

# perfectclamp_wrap.o: perfectclamp_wrap.c
# 	gcc $(SWIGCOMPILELINKSFORPYTHON) -c $(srcdir)/perfectclamp_wrap.c

# perfectclamp_wrap.c: 
# 	$(SWIG) -I$(top_builddir) -python -outdir $(srcdir)/neurospaces/experiment -o $(srcdir)/perfectclamp_wrap.c $(srcdir)/perfectclamp.i

# #-------------- End rules for perfectclamp swig ------------
# #-----------------------------------------------------------

# #-----------------------------------------------------------
# #---------- Rules for building the output swig binding -----
# _output_base.so: output_wrap.o
# 	$(OUTPUT_SWIGLINKCOMMAND_PYTHON)

# output_wrap.o: output_wrap.c
# 	gcc $(SWIGCOMPILELINKSFORPYTHON) -c $(srcdir)/output_wrap.c

# output_wrap.c: 
# 	$(SWIG) -I$(top_builddir) -python -outdir $(srcdir)/neurospaces/experiment -o $(srcdir)/output_wrap.c $(srcdir)/output.i

# #-------------------- End rules for output swig ------------
# #-----------------------------------------------------------



clean-local:
	$(RM) -f $(srcdir)/*.o 
	$(RM) -f $(srcdir)/neurospaces/experiment/*.pyc 
	$(RM) -f $(srcdir)/neurospaces/experiment/*.so
	echo "Removing all compiled byte code files"
	$(RM) `find . -name "*.pyc"`
	$(RM) -rf $(srcdir)/build
	$(RM) -rf $(srcdir)/dist/

CLEANFILES = \
	$(srcdir)/pulsegen_wrap.c \
	$(srcdir)/pulsegen_wrap.o \
	$(srcdir)/neurospaces/experiment/_pulsegen_base.so \
	$(srcdir)/neurospaces/experiment/pulsegen_base.py \
	$(srcdir)/perfectclamp_wrap.c \
	$(srcdir)/perfectclamp_wrap.o \
	$(srcdir)/neurospaces/experiment/_perfectclamp_base.so \
	$(srcdir)/neurospaces/experiment/perfectclamp_base.py \
	$(srcdir)/output_wrap.c \
	$(srcdir)/output_wrap.o \
	$(srcdir)/neurospaces/experiment/_output_base.so \
	$(srcdir)/neurospaces/experiment/output_base.py 
