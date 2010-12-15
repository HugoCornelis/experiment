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
	cp $(srcdir)/experiment_test.pm $(distdir)

swigdir = $(prefix)/glue/swig

#t use $(swigdir) here

perldir = $(prefix)/glue/swig/perl

EXTRA_DIST = experiment.i 

perl_HEADERS = Experiment.pm

perl_DATA = experiment_test.pm SwiggableExperiment.pm SwiggableExperiment.$(PERLLIB_EXTENSION)

BUILT_SOURCES = \
	SwiggableExperiment.pm \
	swiggable_experiment_wrap.c

#t the configuration should say if this depends on neurospaces or not

SwiggableExperiment.pm swiggable_experiment_wrap.c: $(swig_DATA)
	$(SWIG) -I/usr/local/neurospaces/instrumentor -I$(srcdir)/../../.. -I$(top_builddir) -perl5 -makedefault -module SwiggableExperiment -o swiggable_experiment_wrap.c $(srcdir)/experiment.i

#t the configuration should say if this depends on neurospaces or not

swiggable_experiment_wrap.o: swiggable_experiment_wrap.c
	$(SWIGCOMPILECOMMAND_PERL)


SwiggableExperiment.$(PERLLIB_EXTENSION): swiggable_experiment_wrap.o
	$(SWIGLINKCOMMAND_PERL)





all: SwiggableExperiment.$(PERLLIB_EXTENSION)

CLEANFILES = \
	SwiggableExperiment.pm \
	swiggable_experiment_wrap.c \
	swiggable_experiment_wrap.o \
	SwiggableExperiment.$(PERLLIB_EXTENSION)

TESTS = experiment_test.pm

test: check

## following http://www.geocities.com/foetsch/python/swig_linux.htm

#BUILT_SOURCES = $(srcdir)/swiggable_experiment_wrap.c
#SWIG_SOURCES = experiment.i

## other sources
##
## http://sourceware.org/ml/automake/2001-03/msg00395.html
## http://autoconf-archive.cryp.to/macros-by-category.html
## http://sources.redhat.com/ml/automake/2005-02/msg00058.html
## https://www.desy.de/~salvaire/root_wrapper.html

