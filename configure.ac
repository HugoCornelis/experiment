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



# see https://metacpan.org/source/ASPIERS/Stow-v2.1.2/configure.ac

dnl Check for perl on our system.
dnl Call to AC_SUBST(PERL) is implicit
dnl AC_PATH_PROGS([PERL], [perl] [perl5], [false])
dnl if test "x$PERL" = xfalse
dnl then
dnl   AC_MSG_ERROR([Perl not found; check your \$PATH.])
dnl fi
 
# N.B. ${var#pattern} will not work with some shells, such as
# Solaris 10's /bin/sh :-(
#
#   http://www.gnu.org/software/autoconf/manual/autoconf.html#Portable-Shell
#
#   eval `$PERL -V:siteprefix -V:installsitelib`
#   pmdir_relative_path="${installsitelib#$siteprefix/}"
#
# This will work:
#
#   pmdir_relative_path=`echo "${installsitelib}" | sed -e "s!^$siteprefix/!!"`
#
# but this is cleaner:
dnl pmdir_relative_path=`\
dnl   $PERL -MConfig \
dnl         -wle '($_ = $Config{installsitelib})
dnl               =~ s!^\Q$Config{siteprefix}/!!; \
dnl               print'`
 
dnl AC_ARG_WITH(
dnl     [pmdir],
dnl     AS_HELP_STRING(
dnl       [--with-pmdir=DIR],
dnl       [Install Perl modules in DIR]),
dnl     [PMDIR=${withval}],
dnl     [PMDIR='${prefix}'/"$pmdir_relative_path"])
dnl AC_CONFIG_COMMANDS_POST([[
dnl   eval pmdir="$PMDIR"
dnl   cat <<EOF
 
dnl # Perl modules will be installed to $PMDIR
dnl EOF
dnl   if [ "$pmdir" != "$PMDIR" ]; then
dnl     cat <<EOF
dnl # which will expand to
dnl #
dnl #   $pmdir
dnl #
dnl # unless you override the value of prefix at make-time.
dnl EOF
dnl     echo
dnl   fi
dnl ]])
dnl AC_SUBST([PMDIR])




AC_MSG_CHECKING(whether we build universal binaries.)
AC_ARG_WITH(universal,
[AS_HELP_STRING([--with-universal],
              [Build universal binaries on Mac OS X.])],
[ case "$val" in
  no)

    AC_MSG_RESULT(no)
    OSX_UNIVERSAL="no"

    ;;
  *)

    AC_MSG_RESULT(yes)
    OSX_UNIVERSAL="yes"

    CFLAGS="$CFLAGS -arch x86_64 -arch i386 -arch ppc"
    AC_SUBST(CFLAGS)

    ;;
  esac ],
  AC_MSG_RESULT(no)
  OSX_UNIVERSAL="no"
)
AC_SUBST(OSX_UNIVERSAL)


# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB

AC_PATH_PROGS(DOXYGEN,doxygen)
AC_PATH_PROGS(DOT,dot)
AC_PATH_PROGS(SOURCE_HIGHLIGHT,source-highlight)


PERL_MODULES=`(cd perl && find . -name "*.pm" | perl -pe 's/\n/ /g')`
AC_SUBST(PERL_MODULES)

PERL_DATA=`(cd perl && find . -name "numbers-7x11.txt" | perl -pe 's/\n/ /g')`
AC_SUBST(PERL_DATA)

# Checks for programs.

#   check for packaging programs for debian and red hat.
#   no need to bail out if these are missing since they're optional. 
AC_CHECK_PROGS([DPKG_BUILDPACKAGE],dpkg-buildpackage,[NULL])
AC_CHECK_PROGS([DH],dh,[NULL])
AC_CHECK_PROGS([RPMBUILD],rpmbuild,[NULL])

dnl Check for Python and it's headers
AM_PATH_PYTHON(,[],[
   echo "Python is not installed."
   exit -1
])

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
    AC_CHECK_PROG(PERL, perl, NULL, [/usr/bin/perl],[/usr/local/bin/perl /opt/local/bin/perl /sw/bin/perl])
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


    #-- Here we determine the install path for perl modules --
    AC_MSG_CHECKING([Checking the perl module installation path])	
    pmdir_relative_path=`\
      $PERL -MConfig \
        -wle '($_ = $Config{installsitelib})
              =~ s!^\Q$Config{siteprefix}/!!; \
              print'`
 
    AC_ARG_WITH(
      [pmdir],
    AS_HELP_STRING(
      [--with-pmdir=DIR],
      [Install Perl modules in DIR]),
      [PMDIR=${withval}],
      [PMDIR="$pmdir_relative_path"])

    AC_SUBST([PMDIR])
    AC_MSG_RESULT([ $PMDIR ])
    # --- End perl module directory selection --------------


    # determine linker flags for perl embedding


    if test "$OSX_UNIVERSAL" == "yes"; then

      PERL_CCOPTS=`$PERL -MExtUtils::Embed -e ccopts`
      
      PERL_LDADD=`$PERL -MExtUtils::Embed -e ldopts`

      PERL_LDFLAGS=`$PERL -MExtUtils::Embed -e ldopts | perl -pe 's/\-arch\s\S+\s//g'`

    else

      PERL_CCOPTS=`$PERL -MExtUtils::Embed -e ccopts | perl -pe 's/\-arch\s\S+\s//g'`
    
      PERL_LDADD=`$PERL -MExtUtils::Embed -e ldopts | perl -pe 's/\-arch\s\S+\s//g'`

      PERL_LDFLAGS=`$PERL -MExtUtils::Embed -e ldopts | perl -pe 's/\-arch\s\S+\s//g'`

    fi

    AC_SUBST(PERL_CCOPTS)
    AC_SUBST(PERL_LDADD)
    AC_SUBST(PERL_LDFLAGS)


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


    if test "$OSX_UNIVERSAL" == "yes"; then 

      SWIGLDLINKSFORPERL='-dynamic -undefined suppress -flat_namespace -L${top_builddir} 	-lexperiment 	-L/usr/local/lib/model-container 	-lneurospacesread 	-L/usr/local/neurospaces/instrumentor/hierarchy/output/symbols/ -lsymbol_algorithms -L$(top_builddir)/../../../../model-container/source/snapshots/0/algorithms/event -levent_algorithms -lncurses -lreadline $(PERL_LDFLAGS)'

    else

      SWIGLDLINKSFORPERL='-L${top_builddir} 	-lexperiment 	-L/usr/local/lib/model-container 	-lneurospacesread 	-L/usr/local/neurospaces/instrumentor/hierarchy/output/symbols/ -lsymbol_algorithms -lncurses -lreadline -bundle -undefined suppress -flat_namespace'

    fi

    AC_SUBST(SWIGLDLINKSFORPERL)

    case "$host_os" in
	darwin10*)
	    dnl This is snow leopard

	    if test "$OSX_UNIVERSAL" == "yes"; then

              SWIGLINKCOMMAND_PERL='$(AR) $(SWIGLDLINKSFORPERL) -o SwiggableExperiment.$(PERLLIB_EXTENSION) swiggable_experiment_wrap.o'

            else

              SWIGLINKCOMMAND_PERL='ld $(SWIGLDLINKSFORPERL) -o SwiggableExperiment.$(PERLLIB_EXTENSION) /usr/lib/bundle1.o  swiggable_experiment_wrap.o'

            fi

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

    SWIGLDLINKSFORPYTHON='$(PYTHON_LDFLAGS) -L${top_builddir} -lexperiment  -L/usr/local/lib/model-container -lneurospacesread -L/usr/local/lib/model-container/lib -lsymbol_algorithms -lncurses -lreadline  -framework Python -flat_namespace -undefined suppress -bundle -bundle_loader $(PYTHON_EXE)'
    AC_SUBST(SWIGLDLINKSFORPYTHON)


    case "$host_os" in
	darwin10*)
	    dnl This is snow leopard

            SWIGLINKCOMMAND_PYTHON='ld 	$(SWIGLDLINKSFORPYTHON) -o _SwiggableExperiment.so /usr/lib/bundle1.o swiggable_experiment_wrap.o'
            AC_SUBST(SWIGLINKCOMMAND_PYTHON)


            PULSEGEN_SWIGLINKCOMMAND_PYTHON='ld $(SWIGLDLINKSFORPYTHON) -o $(srcdir)/neurospaces/experiment/_pulsegen_base.so /usr/lib/bundle1.o pulsegen_wrap.o'
            AC_SUBST(PULSEGEN_SWIGLINKCOMMAND_PYTHON)

            PERFECTCLAMP_SWIGLINKCOMMAND_PYTHON='ld $(SWIGLDLINKSFORPYTHON) -o $(srcdir)/neurospaces/experiment/_perfectclamp_base.so /usr/lib/bundle1.o perfectclamp_wrap.o'
            AC_SUBST(PERFECTCLAMP_SWIGLINKCOMMAND_PYTHON)

            OUTPUT_SWIGLINKCOMMAND_PYTHON='ld $(SWIGLDLINKSFORPYTHON) -o $(srcdir)/neurospaces/experiment/_output_base.so /usr/lib/bundle1.o output_wrap.o'
            AC_SUBST(OUTPUT_SWIGLINKCOMMAND_PYTHON)

	;;
	*)
            SWIGLINKCOMMAND_PYTHON='ld 	$(SWIGLDLINKSFORPYTHON) -o _SwiggableExperiment.so  swiggable_experiment_wrap.o'
            AC_SUBST(SWIGLINKCOMMAND_PYTHON)


            PULSEGEN_SWIGLINKCOMMAND_PYTHON='ld $(SWIGLDLINKSFORPYTHON) -o $(srcdir)/neurospaces/experiment/_pulsegen_base.so  pulsegen_wrap.o'
            AC_SUBST(PULSEGEN_SWIGLINKCOMMAND_PYTHON)	

            PERFECTCLAMP_SWIGLINKCOMMAND_PYTHON='ld $(SWIGLDLINKSFORPYTHON) -o $(srcdir)/neurospaces/experiment/_perfectclamp_base.so  perfectclamp_wrap.o'
            AC_SUBST(PERFECTCLAMP_SWIGLINKCOMMAND_PYTHON)

            OUTPUT_SWIGLINKCOMMAND_PYTHON='ld $(SWIGLDLINKSFORPYTHON) -o $(srcdir)/neurospaces/experiment/_output_base.so  output_wrap.o'
            AC_SUBST(OUTPUT_SWIGLINKCOMMAND_PYTHON)	

        ;;
    esac 


    AC_MSG_RESULT([done setting Mac OSX settings.])
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

    #-- Here we determine the install path for perl modules --
    AC_MSG_CHECKING([Checking the perl module installation path])	
    pmdir_relative_path=`\
      $PERL -MConfig \
        -wle '($_ = $Config{installsitelib})
              =~ s!^\Q$Config{siteprefix}/!!; \
              print'`
 
    AC_ARG_WITH(
      [pmdir],
    AS_HELP_STRING(
      [--with-pmdir=DIR],
      [Install Perl modules in DIR]),
      [PMDIR=${withval}],
      [PMDIR='${prefix}'/"$pmdir_relative_path"])

    AC_SUBST([PMDIR])
    AC_MSG_RESULT([ $PMDIR ])
    # --- End perl module directory selection --------------


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


    SWIGLDLINKSFORPERL='-g -shared swiggable_experiment_wrap.o -L${top_builddir} -lexperiment -L/usr/local/lib/model-container -lneurospacesread -L/usr/local/lib/model-container/lib -lsymbol_algorithms -lncurses -lreadline -lhistory -fPIC'
    AC_SUBST(SWIGLDLINKSFORPERL)

    SWIGLINKCOMMAND_PERL='gcc $(SWIGLDLINKSFORPERL) -o SwiggableExperiment.$(PERLLIB_EXTENSION)'
    AC_SUBST(SWIGLINKCOMMAND_PERL)


    PYTHONLIB_EXTENSION='so'
    AC_SUBST(PYTHONLIB_EXTENSION)

    SWIGCOMPILELINKSFORPYTHON='-g -DPRE_PROTO_TRAVERSAL $(PYTHON_INCLUDE) -I/usr/local/neurospaces/instrumentor -I${top_builddir} -I${top_builddir}/experiment -fPIC -Xlinker -export-dynamic'
    AC_SUBST(SWIGCOMPILELINKSFORPYTHON)

   SWIGCOMPILECOMMAND_PYTHON='gcc $(SWIGCOMPILELINKSFORPYTHON) -c swiggable_experiment_wrap.c'
   AC_SUBST(SWIGCOMPILECOMMAND_PYTHON)

    SWIGLDLINKSFORPYTHON='-g -L${top_builddir} -lexperiment -L/usr/local/lib/model-container -lneurospacesread -L/usr/local/lib/model-container/lib -lsymbol_algorithms -lncurses -lreadline -lhistory -fPIC -Xlinker -export-dynamic $(PYTHON_LDFLAGS)'
    AC_SUBST(SWIGLDLINKSFORPYTHON)

    SWIGLINKCOMMAND_PYTHON='gcc -shared swiggable_experiment_wrap.o $(SWIGLDLINKSFORPYTHON) -o _SwiggableExperiment.$(PYTHONLIB_EXTENSION)'
    AC_SUBST(SWIGLINKCOMMAND_PYTHON)


    PULSEGEN_SWIGLINKCOMMAND_PYTHON='gcc -shared pulsegen_wrap.o $(SWIGLDLINKSFORPYTHON) -o $(srcdir)/neurospaces/experiment/_pulsegen_base.so'
    AC_SUBST(PULSEGEN_SWIGLINKCOMMAND_PYTHON)


    PERFECTCLAMP_SWIGLINKCOMMAND_PYTHON='gcc -shared perfectclamp_wrap.o $(SWIGLDLINKSFORPYTHON) -o $(srcdir)/neurospaces/experiment/_perfectclamp_base.so'
    AC_SUBST(PERFECTCLAMP_SWIGLINKCOMMAND_PYTHON)


    OUTPUT_SWIGLINKCOMMAND_PYTHON='gcc -shared output_wrap.o $(SWIGLDLINKSFORPYTHON) -o $(srcdir)/neurospaces/experiment/_output_base.so'
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

