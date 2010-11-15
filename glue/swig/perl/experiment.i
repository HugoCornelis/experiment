/* -*- C -*- */

/* swig -perl5 -makedefault -module Experiment experiment.i */
/* gcc -c experiment_wrap.c `perl -MExtUtils::Embed -e ccopts`  */
/* gcc -shared experiment_wrap.o -L. -lexperiment -o experiment.so */

%module SwiggableExperiment

/* %typemap(in) struct Neurospaces * { */

/*     //- cast to _p_Neurospaces */

/*     if (SvOK($input)) */
/*     { */
/* 	$1 = SvRV($input); */
/*     } */
/* } */

/* %typemap(out) struct Neurospaces * { */

/*     //- cast to _p_Neurospaces */

/*     $1 = $input; */
/* } */

/* %typemap(ruby,in) (int size, int *ary) { */
/*    int i; */
/*    if (!rb_obj_is_kind_of($input,rb_cArray)) */
/*      rb_raise(rb_eArgError, "Expected Array of Integers"); */
/*    $1 = RARRAY($input)->len; */
/*    $2 = malloc($1*sizeof(int)); */
/*    for (i=0; i<$1; ++i) */
/*      ($2)[i] = NUM2INT(RARRAY($input)->ptr[i]); */
/* } */
/* %typemap(freearg) (int size, int *ary) { */
/*     if ($2) free($2); */
/* } */

/* %typemap(memberin) int [ANY] */
/* { */
/*     int i; */
/*     for (i = 0 ; i < $1_dim0 ; i++) */
/*     { */
/* 	$1[i] = $input[i]; */
/*     } */
/* } */

%{
#include "experiment/config.h"
#include "experiment/output.h"
#include "experiment/perfectclamp.h"
#include "experiment/pulsegen.h"

%}

%inline %{


/* int iHECCER_DUMP_ALL = HECCER_DUMP_ALL; */

/* int iHECCER_DUMP_ALL_EXTENDED = HECCER_DUMP_ALL_EXTENDED; */

/* int iHECCER_DUMP_ALL_REDUCED = HECCER_DUMP_ALL_REDUCED; */


/* void piC2m_set(struct Intermediary *pim, int *piC2m) */
/* { */
/*     pim->piC2m = piC2m; */
/* } */

/// integer array handling

//t use array generics, see carray.i

int *int_array(int size)
{
    return (int *) malloc(sizeof(int) * size);
}
void int_destroy(int *a)
{
    free(a);
}
void int_set(int *a, int i, int val)
{
    a[i] = val;
}
int int_get(int *a, int i)
{
    return a[i];
}

/// double array handling

//t use array generics, see carray.i

double *double_array(int size)
{
    return (double *) malloc(sizeof(double) * size);
}
void double_destroy(double *a)
{
    free(a);
}
void double_set(double *a, int i, double val)
{
    a[i] = val;
}
double double_get(double *a, int i)
{
    return a[i];
}

/// scheduler optimization

typedef int (*DriverMethod)(void *pvData);

void * output_generator_get_driver_data(struct OutputGenerator *pog)
{
    return((void *)pog);
}


void * output_generator_get_driver_method(struct OutputGenerator *pog)
{
    return((void *)OutputGeneratorTimedStep);
}


void * output_generator_get_event_processor(struct OutputGenerator *pog)
{
    return((void *)OutputGeneratorTimedStep);
}


void * perfect_clamp_get_driver_data(struct simobj_PerfectClamp *ppc)
{
    return((void *)ppc);
}


void * perfect_clamp_get_driver_method(struct simobj_PerfectClamp *ppc)
{
    return((void *)PerfectClampSingleStep);
}

void * pulse_gen_get_driver_data(struct simobj_PulseGen *ppg)
{
    return((void *)ppg);
}


void * pulse_gen_get_driver_method(struct simobj_PulseGen *ppg)
{
    return((void *)PulseGenSingleStep);
}


%}

%include "experiment/config.h"
%include "experiment/output.h"
%include "experiment/perfectclamp.h"
%include "experiment/pulsegen.h"


