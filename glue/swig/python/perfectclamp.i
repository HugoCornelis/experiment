%module perfectclamp_base	

/***************************************************
* Start C code block
***************************************************/
%{
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "experiment/perfectclamp.h"

%}
/***************************************************
* End C code block
***************************************************/


%import "cpointer.i"

%pointer_functions(double,pdouble);

%import "typemaps.i"



%typemap(out) int 
{
  $result = PyInt_FromLong($1);
}


%typemap(in) void *pvVoltage
{

  double dTemp = PyFloat_AsDouble($input);

  double *dOutput = (double*)malloc(sizeof(double));

  *dOutput = dTemp;

  $1 = (double*)dOutput;

}



extern int PerfectClampAddVariable(struct simobj_PerfectClamp * ppc, void *pvVoltage);

extern int PerfectClampSingleStep(struct simobj_PerfectClamp * ppc, double dTime);

extern int PerfectClampFinish(struct simobj_PerfectClamp * ppc);

extern int PerfectClampInitiate(struct simobj_PerfectClamp * ppc);

extern struct simobj_PerfectClamp * PerfectClampNew(char *pcName);

extern int PerfectClampSetFields(struct simobj_PerfectClamp * ppc, double dCommand, char *pcFilename);


/*------------------------------------------------------------------------
* Grab the original header file so SWIG can import the prototypes and
* create low level data classes out of the member structs. Apparently this
* line must be at the end of the file otherwise SWIG will ignore my typemaps.
*------------------------------------------------------------------------*/
%include "experiment/perfectclamp.h"