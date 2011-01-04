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


SUBDIRS = 


#t the following rules are crap complete, would be good if a auto
#t expert takes a look at it.

dist-hook:
	cp $(srcdir)/experiment.i $(distdir)
	$(INSTALL) -d $(distdir)/tests
	cp $(srcdir)/tests/*.py $(distdir)/tests

swigdir = $(prefix)/glue/swig

#t use $(swigdir) here

pythondir = $(prefix)/glue/swig/python

nobase_python_HEADERS = experiment/__init__.py

python_DATA =  SwiggableExperiment.py _SwiggableExperiment.$(PYTHONLIB_EXTENSION) tests/experiment_test.py

EXTRA_DIST = experiment.i

BUILT_SOURCES = \
	SwiggableExperiment.py \
	swiggable_experiment_wrap.c

SwiggableExperiment.py swiggable_experiment_wrap.c: $(swig_DATA)
	$(SWIG) -I$(builddir)../../.. -I$(srcdir)/../../.. -python -makedefault -module SwiggableExperiment -o swiggable_experiment_wrap.c $(srcdir)/experiment.i 

swiggable_experiment_wrap.o: swiggable_experiment_wrap.c
	$(SWIGCOMPILECOMMAND_PYTHON)


_SwiggableExperiment.$(PYTHONLIB_EXTENSION): swiggable_experiment_wrap.o
	$(SWIGLINKCOMMAND_PYTHON)

all: _SwiggableExperiment.$(PYTHONLIB_EXTENSION)

CLEANFILES = \
	SwiggableExperiment.py \
	swiggable_experiment_wrap.c \
	swiggable_experiment_wrap.o \
	_SwiggableExperiment.$(PYTHONLIB_EXTENSION)


