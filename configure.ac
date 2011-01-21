dnl
dnl Experiment : a compartmental solver that implements efficient Crank-Nicolson
dnl integration for neuronal models.
dnl

dnl //////////////////////////////////////////////////////////////////////////
dnl 
dnl Experiment : testbed C implementation
dnl 
dnl Copyright (C) 2006-2008 Hugo Cornelis
dnl 
dnl functional ideas ..	Hugo Cornelis, hugo.cornelis@gmail.com
dnl 
dnl coding ............	Hugo Cornelis, hugo.cornelis@gmail.com
dnl 
dnl //////////////////////////////////////////////////////////////////////////

# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)

# $Format: "AC_INIT(${package}, ${major}.${minor}.${micro}-${label}, ${email})"$
AC_INIT(experiment, 0.0.0-alpha, hugo.cornelis@gmail.com)

dnl Obtain operating system information.
AC_CANONICAL_TARGET

AC_CONFIG_SRCDIR([output.c])
AC_CONFIG_HEADER([experiment/config.h])

dnl dnl all subdirs

dnl AC_CONFIG_SUBDIRS(tests/code glue)

# $Format: "AM_INIT_AUTOMAKE(${package}, ${major}.${minor}.${micro}-${label})"$
AM_INIT_AUTOMAKE(experiment, 0.0.0-alpha)


# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB

AC_PATH_PROGS(DOXYGEN,doxygen)
AC_PATH_PROGS(DOT,dot)
AC_PATH_PROGS(SOURCE_HIGHLIGHT,source-highlight)

dnl Check for Python and it's headers
AM_PATH_PYTHON(,[],[
   echo "Python is not installed."
   exit -1
])

AC_MSG_CHECKING([Python prefix is])
  AC_MSG_RESULT(['$PYTHON_PREFIX'])

dnl
dnl This is a forced fix to make sure a mac installation gets the proper
dnl python executable. Unless the macpython installer was run, the autotools
dnl python macro won't look for python in the proper path. 
dnl 
case "$host_os" in
  darwin*)

    PYTHON_PREFIX='/Library/Frameworks/Python.framework/Versions/Current'
    AC_SUBST(PYTHON_PREFIX)
    
    PYTHON=$PYTHON_PREFIX/bin/python
    AC_SUBST(PYTHON)

    AC_CHECK_PROG(HAVE_PYTHON_CONFIG, python-config, "yes", "no",[$PYTHON_PREFIX/bin])
    AC_MSG_CHECKING([python-config present])
    AC_MSG_RESULT([$HAVE_PYTHON_CONFIG])
  ;;
  *)

    AC_CHECK_PROG(HAVE_PYTHON_CONFIG, python-config, "yes", "no")
    AC_MSG_CHECKING([python-config present])
    AC_MSG_RESULT([$HAVE_PYTHON_CONFIG])

  ;;
esac




if test "$HAVE_PYTHON_CONFIG" == "yes"; then

  # This is to set the proper python-config for getting the 
  # proper flags and prefixes

  
  AC_MSG_CHECKING([Python config location])
  PYTHON_CONFIG=$PYTHON-config
  AC_SUBST(PYTHON_CONFIG)
  AC_MSG_RESULT([$PYTHON_CONFIG])


  AC_MSG_CHECKING([Python prefix is])
  PYTHON_PREFIX=`$PYTHON_CONFIG --prefix`
  AC_SUBST(PYTHON_PREFIX)
  AC_MSG_RESULT([$PYTHON_PREFIX])


  AC_MSG_CHECKING([Python includes are])
  PYTHON_INCLUDE=`$PYTHON_CONFIG --include`
  AC_SUBST(PYTHON_INCLUDE)
  AC_MSG_RESULT([$PYTHON_INCLUDE])


  AC_MSG_CHECKING([Python libs are])
  PYTHON_LIBS=`$PYTHON_CONFIG --libs`
  AC_SUBST(PYTHON_LIBS)
  AC_MSG_RESULT([$PYTHON_LIBS])


  AC_MSG_CHECKING([Python cflags are])
  PYTHON_CFLAGS=`$PYTHON_CONFIG --cflags`
  AC_SUBST(PYTHON_CFLAGS)
  AC_MSG_RESULT([$PYTHON_CFLAGS])


  AC_MSG_CHECKING([Python ldflags are])
  PYTHON_LDFLAGS=`$PYTHON_CONFIG --ldflags`
  AC_SUBST(PYTHON_LDFLAGS)
  AC_MSG_RESULT([$PYTHON_LDFLAGS])

  AC_MSG_CHECKING([Python executable is])
  PYTHON_EXE=$PYTHON_PREFIX/bin/python
  AC_SUBST(PYTHON_EXE)
  AC_MSG_RESULT([$PYTHON_EXE])


else

  # This is legacy to make sure that systems with python
  # that do not have python-config can get the python info
  # This has always been a bit error prone, particularly if the 
  # user has more than one python version installed.

  PYTHON_PREFIX=`echo $PYTHON | sed -e "s,[[^/]]*$,,;s,/$,,;s,^$,.,"`
  PYTHON_PREFIX=`echo $PYTHON_PREFIX | sed -e "s,[[^/]]*$,,;s,/$,,;s,^$,.,"`
  AC_SUBST(PYTHON_PREFIX)


  AC_MSG_CHECKING([Python prefix is])
  AC_MSG_RESULT([$PYTHON_PREFIX])


  PYTHON_HEADERS=$(PYTHON_PREFIX)/include/python$(PYTHON_VERSION)
  AC_SUBST(PYTHON_HEADERS)

  AC_MSG_CHECKING([Python includes are])
  PYTHON_INCLUDE='-I$(PYTHON_HEADERS)'
  AC_SUBST(PYTHON_INCLUDE)
  AC_MSG_RESULT([$PYTHON_INCLUDE])

  AC_MSG_CHECKING([Python ldflags are])
  PYTHON_LIBS='-L$(pyexecdir)/../config'
  AC_SUBST(PYTHON_LIBS)
  AC_MSG_RESULT([$PYTHON_LIBS])
				


  PYTHON_EXE='$(PYTHON_PREFIX)/bin/python'
  AC_SUBST(PYTHON_EXE)
fi



# Checks for options

AC_ARG_ENABLE(random, AS_HELP_STRING([--with-random   use the builtin random number generator (recommended not to use the builtin)]) )
if test x"$with_random" == "xyes" ; then

    echo "USING THE BUILTIN RANDOM NUMBER GENERATOR (recommended not to use the builtin)"

    AC_DEFINE(RANDOM, (RAND_MAX * ran1(0)), [use the numerical recipes ran1() random number generator (not recommended)])

else

    echo "not using the builtin random number generator (recommended not to use the builtin)"

    AC_DEFINE(RANDOM, random(), [use the 4.3BSD random() random number generator])

fi

AC_ARG_ENABLE(sglib, AS_HELP_STRING([--with-sglib   use the sglib based event queue]) )
if test x"$with_sglib" == "xyes" ; then

    echo "using the sglib based event queue"

    AC_DEFINE(USE_SGLIB, 1, [use the sglib based event queue])

else

    echo "not using the sglib based event queue"

    AC_DEFINE(USE_SGLIB, 0, [use the builtin event queue])

fi

AC_ARG_ENABLE(random, AS_HELP_STRING([--with-id-disassem   output serial ids of compartments in the disassembler]) )
if test x"$with_id_disassem" == "xyes" ; then

    echo "outputting serial ids of compartments in the disassembler"

    AC_DEFINE(USE_ID_DISASSEM, 1, [outputting serial ids of compartments in the disassembler])

else

    echo "not outputting serial ids of compartments in the disassembler"

    AC_DEFINE(USE_ID_DISASSEM, 0, [not outputting serial ids of compartments in the disassembler])

fi

# integrators: integrate with model-container of neurospaces ?

AC_ARG_ENABLE(neurospaces, AS_HELP_STRING([--without-neurospaces   don't use neurospaces as modeling component]) )

EXPERIMENT_INTEGRATORS=

AC_SUBST(EXPERIMENT_INTEGRATORS)

# enable source typing to compute aggregate currents ?

AC_ARG_ENABLE(source-typing, AS_HELP_STRING([--without-source-typing   don't use model source typing for calculation of aggregate currents]) )

AC_MSG_CHECKING([Use source typing])

if test x"$with_source_typing" == "xno" ; then

    AC_MSG_RESULT([no])

else

    AC_MSG_RESULT([yes])

    AC_DEFINE(EXPERIMENT_SOURCE_TYPING, 1, [use model source typing])
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([float.h limits.h locale.h stdlib.h string.h])

# see for example http://www.nabble.com/AC_CHECK_HEADER-%2B-extra-compiler-flags-td10318845.html#a10319498

SAVE_CPPFLAGS=$CPPFLAGS
CPPFLAGS="-I/usr/local/neurospaces/instrumentor -DPRE_PROTO_TRAVERSAL"

AC_CHECK_HEADERS([neurospaces/neurospaces.h],[],
[echo "Error: neurospaces headers are not installed in standard location."])

CPPFLAGS=$SAVE_CPPFLAGS

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST

# Checks for library functions.
AC_CHECK_FUNCS([localeconv strchr])


dnl This does macro substitutions for certain operating systems and 
dnl architectures 
AC_MSG_CHECKING([OS specifics...])
AC_MSG_RESULT([Host is running $host_os.])
dnl {{{
case "$host_os" in
  darwin*)
    #
    # These are settings specific to MAC OS X.
    #
    dnl {{{{
    AC_MSG_CHECKING([Using libtool for MAC OS X.])
    AR=libtool
    ARFLAGS='-static -c -o'
    AC_SUBST(AR)
    AC_SUBST(ARFLAGS)
    AC_MSG_RESULT([$AR $ARFLAGS])

    dnl On the MAC we will only look for perl in the standard location for the Apple
    dnl native perl. 
    AC_CHECK_PROG(PERL, perl, NULL, [/usr/bin/perl],[/usr/local/bin/perl /opt/local/bin/perl])
    AC_SUBST(PERL)

    AC_MSG_CHECKING([Checking the perl installation])
    case $PERL in
      NULL*)
        echo ""
        echo "Error, the standard version of perl for Mac OSX is not found."
        echo "Non standard versions of perl are known to cause problems."
        AC_ERROR([Perl for Mac OSX is not found.])
        ;;
      *)
        AC_MSG_RESULT([ $PERL ])
        ;;
    esac

    # determine linker flags for perl embedding

    PERL_CCOPTS=`$PERL -MExtUtils::Embed -e ccopts | $PERL -pe 's/\-arch\s\S+\s//g'`
    AC_SUBST(PERL_CCOPTS)

    AC_CHECK_PROG(SWIG, swig, NULL, [/usr/bin/swig],[/usr/local/bin/swig /opt/local/bin/swig])
    AC_SUBST(SWIG)

    AC_MSG_CHECKING([Checking for swig])
    case $SWIG in
      NULL*)
        echo ""
        echo "Error, the standard version of swig for Mac OSX is not found."
        echo "Non standard versions of swig are known to cause problems."
        AC_ERROR([Swig for Mac OSX is not found.])
        ;;
      *)
        AC_MSG_RESULT([ $SWIG ])
        ;;
    esac

    PERLLIB_EXTENSION='dylib'
    AC_SUBST(PERLLIB_EXTENSION)

    SWIGCOMPILELINKSFORPERL='-I/usr/local/neurospaces/instrumentor -I${top_builddir} -ffloat-store -include ${top_builddir}/experiment/config.h -g -O0 -dynamic -fPIC $(PERL_CCOPTS)'
    AC_SUBST(SWIGCOMPILELINKSFORPERL)

    SWIGCOMPILECOMMAND_PERL='gcc $(SWIGCOMPILELINKSFORPERL) -c swiggable_experiment_wrap.c'
    AC_SUBST(SWIGCOMPILECOMMAND_PERL)

 

    SWIGLDLINKSFORPERL='-L${top_builddir} 	-lexperiment 	-L/usr/local/lib/model-container 	-lneurospacesread 	-L/usr/local/neurospaces/instrumentor/hierarchy/output/symbols/ -lsymbol_algorithms -lncurses -lreadline -bundle -undefined suppress -flat_namespace'
    AC_SUBST(SWIGLDLINKSFORPERL)

    case "$host_os" in
	darwin10*)
	    dnl This is snow leopard
            SWIGLINKCOMMAND_PERL='ld 	$(SWIGLDLINKSFORPERL) 	-o SwiggableExperiment.$(PERLLIB_EXTENSION) /usr/lib/bundle1.o  swiggable_experiment_wrap.o'
            AC_SUBST(SWIGLINKCOMMAND_PERL)


	;;    
	*)
            SWIGLINKCOMMAND_PERL='ld 	$(SWIGLDLINKSFORPERL) 	-o SwiggableExperiment.$(PERLLIB_EXTENSION) 	swiggable_experiment_wrap.o'
            AC_SUBST(SWIGLINKCOMMAND_PERL)
	;;
    esac 





    PYTHONLIB_EXTENSION='so'
    AC_SUBST(PYHONLIB_EXTENSION)

    SWIGCOMPILELINKSFORPYTHON='$(PYTHON_INCLUDE) -I/usr/local/neurospaces/instrumentor -I${top_builddir}'
    AC_SUBST(SWIGCOMPILELINKSFORPYTHON)

    SWIGCOMPILECOMMAND_PYTHON='gcc         $(SWIGCOMPILELINKSFORPYTHON)         -c swiggable_experiment_wrap.c'
    AC_SUBST(SWIGCOMPILECOMMAND_PYTHON)

    SWIGLDLINKSFORPYTHON='$(PYTHON_LDFLAGS) -L${top_builddir} -lexperiment  -L/usr/local/lib/model-container -lneurospacesread -L/usr/local/lib/neurospaces/algorithms/symbol -lsymbol_algorithms -lncurses -lreadline  -framework Python -flat_namespace -undefined suppress -bundle -bundle_loader $(PYTHON_EXE)'
    AC_SUBST(SWIGLDLINKSFORPYTHON)


    case "$host_os" in
	darwin10*)
	    dnl This is snow leopard

            SWIGLINKCOMMAND_PYTHON='ld 	$(SWIGLDLINKSFORPYTHON) -o _SwiggableExperiment.so /usr/lib/bundle1.o swiggable_experiment_wrap.o'
            AC_SUBST(SWIGLINKCOMMAND_PYTHON)


            PULSEGEN_SWIGLINKCOMMAND_PYTHON='ld $(SWIGLDLINKSFORPYTHON) -o $(srcdir)/experiment/_pulsegen_base.so /usr/lib/bundle1.o pulsegen_wrap.o'
            AC_SUBST(PULSEGEN_SWIGLINKCOMMAND_PYTHON)

            PERFECTCLAMP_SWIGLINKCOMMAND_PYTHON='ld $(SWIGLDLINKSFORPYTHON) -o $(srcdir)/experiment/_perfectclamp_base.so /usr/lib/bundle1.o perfectclamp_wrap.o'
            AC_SUBST(PERFECTCLAMP_SWIGLINKCOMMAND_PYTHON)

            OUTPUT_SWIGLINKCOMMAND_PYTHON='ld $(SWIGLDLINKSFORPYTHON) -o $(srcdir)/experiment/_output_base.so /usr/lib/bundle1.o output_wrap.o'
            AC_SUBST(OUTPUT_SWIGLINKCOMMAND_PYTHON)

	;;
	*)
            SWIGLINKCOMMAND_PYTHON='ld 	$(SWIGLDLINKSFORPYTHON) -o _SwiggableExperiment.so  swiggable_experiment_wrap.o'
            AC_SUBST(SWIGLINKCOMMAND_PYTHON)


            PULSEGEN_SWIGLINKCOMMAND_PYTHON='ld $(SWIGLDLINKSFORPYTHON) -o $(srcdir)/experiment/_pulsegen_base.so  pulsegen_wrap.o'
            AC_SUBST(PULSEGEN_SWIGLINKCOMMAND_PYTHON)	

            PERFECTCLAMP_SWIGLINKCOMMAND_PYTHON='ld $(SWIGLDLINKSFORPYTHON) -o $(srcdir)/experiment/_perfectclamp_base.so  perfectclamp_wrap.o'
            AC_SUBST(PERFECTCLAMP_SWIGLINKCOMMAND_PYTHON)

            OUTPUT_SWIGLINKCOMMAND_PYTHON='ld $(SWIGLDLINKSFORPYTHON) -o $(srcdir)/experiment/_output_base.so  output_wrap.o'
            AC_SUBST(OUTPUT_SWIGLINKCOMMAND_PYTHON)	

        ;;
    esac 


    AC_MSG_RESULT([done.])
    dnl }}} end MAC OS X settings
    ;;




  *)
    #
    # These are settings specific to Linux/Unix POSIX systems.
    #
    dnl {{{
    AR='ar'
    ARFLAGS='cru'
    AC_SUBST(AR)
    AC_SUBST(ARFLAGS)
    AC_MSG_CHECKING([ranlib invocation ])
    AC_MSG_RESULT([$AR $ARFLAGS])

    dnl Check for some needed programs.
    AC_CHECK_PROGS(PERL, perl5 perl)
    AC_SUBST(PERL)

    AC_CHECK_PROGS(SWIG, swig)
    AC_SUBST(SWIG)


    PERL_CCOPTS=`$PERL -MExtUtils::Embed -e ccopts`
    AC_SUBST(PERL_CCOPTS)

    PERLLIB_EXTENSION='so'
    AC_SUBST(PERLLIB_EXTENSION)


    SWIGCOMPILELINKSFORPERL='-I/usr/local/neurospaces/instrumentor -I${top_builddir} -ffloat-store -include experiment/config.h -g -O0 -fPIC $(PERL_CCOPTS)'
    AC_SUBST(SWIGCOMPILELINKSFORPERL)

    SWIGCOMPILECOMMAND_PERL='gcc $(SWIGCOMPILELINKSFORPERL) -c swiggable_experiment_wrap.c'
    AC_SUBST(SWIGCOMPILECOMMAND_PERL)


    SWIGLDLINKSFORPERL='-g -shared swiggable_experiment_wrap.o -L${top_builddir} -lexperiment -L/usr/local/lib/model-container -lneurospacesread -L/usr/local/lib/neurospaces/algorithms/symbol -lsymbol_algorithms -lncurses -lreadline -lhistory -fPIC'
    AC_SUBST(SWIGLDLINKSFORPERL)

    SWIGLINKCOMMAND_PERL='gcc $(SWIGLDLINKSFORPERL) -o SwiggableExperiment.$(PERLLIB_EXTENSION)'
    AC_SUBST(SWIGLINKCOMMAND_PERL)


    PYTHONLIB_EXTENSION='so'
    AC_SUBST(PYTHONLIB_EXTENSION)

    SWIGCOMPILELINKSFORPYTHON='-g -DPRE_PROTO_TRAVERSAL $(PYTHON_INCLUDE) -I/usr/local/neurospaces/instrumentor -I${top_builddir} -I${top_builddir}/experiment -fPIC -Xlinker -export-dynamic'
    AC_SUBST(SWIGCOMPILELINKSFORPYTHON)

   SWIGCOMPILECOMMAND_PYTHON='gcc $(SWIGCOMPILELINKSFORPYTHON) -c swiggable_experiment_wrap.c'
   AC_SUBST(SWIGCOMPILECOMMAND_PYTHON)

    SWIGLDLINKSFORPYTHON='-g -L${top_builddir} -lexperiment -L/usr/local/lib/model-container -lneurospacesread -L/usr/local/lib/neurospaces/algorithms/symbol -lsymbol_algorithms -lncurses -lreadline -lhistory -fPIC -Xlinker -export-dynamic $(PYTHON_LDFLAGS)'
    AC_SUBST(SWIGLDLINKSFORPYTHON)

    SWIGLINKCOMMAND_PYTHON='gcc -shared swiggable_experiment_wrap.o $(SWIGLDLINKSFORPYTHON) -o _SwiggableExperiment.$(PYTHONLIB_EXTENSION)'
    AC_SUBST(SWIGLINKCOMMAND_PYTHON)


    PULSEGEN_SWIGLINKCOMMAND_PYTHON='gcc -shared pulsegen_wrap.o $(SWIGLDLINKSFORPYTHON) -o $(srcdir)/experiment/_pulsegen_base.so'
    AC_SUBST(PULSEGEN_SWIGLINKCOMMAND_PYTHON)


    PERFECTCLAMP_SWIGLINKCOMMAND_PYTHON='gcc -shared perfectclamp_wrap.o $(SWIGLDLINKSFORPYTHON) -o $(srcdir)/experiment/_perfectclamp_base.so'
    AC_SUBST(PERFECTCLAMP_SWIGLINKCOMMAND_PYTHON)


    OUTPUT_SWIGLINKCOMMAND_PYTHON='gcc -shared output_wrap.o $(SWIGLDLINKSFORPYTHON) -o $(srcdir)/experiment/_output_base.so'
    AC_SUBST(OUTPUT_SWIGLINKCOMMAND_PYTHON)
    dnl }}} End of General Settings

  ;;
esac
dnl }}}



# determine the test library to install

TEST_SPECIFICATION_FILES=`/bin/echo -n "tests/introduction.html " && find tests/specifications -name "*.t" | perl -pe 's/\n/ /g' && find tests/specifications/strings -name "*.txt" | perl -pe 's/\n/ /g'`
AC_SUBST(TEST_SPECIFICATION_FILES)


AC_CONFIG_FILES([Makefile
                 glue/Makefile
                 glue/swig/perl/Makefile
		 glue/swig/python/Makefile
		 glue/swig/python_old/Makefile
                 tests/Makefile])
AC_OUTPUT

