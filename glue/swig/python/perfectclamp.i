%module perfectclamp_base	

/***************************************************
* Start C code block
***************************************************/
%{
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "experiment/perfectclamp.h"

double GetVariableValue(struct simobj_PerfectClamp *ppc);
/* double GetCommand(struct simobj_PerfectClamp *ppc); */


double GetVariableValue(struct simobj_PerfectClamp *ppc)
{
  if (!ppc || !ppc->pdVariable)
  {
    return FLT_MAX;
  }

  return (*ppc->pdVariable);

}

/* double GetCommand(struct simobj_PerfectClamp *ppc) */
/* { */
/*   if (!ppc || !ppc->pdVariable) */
/*   { */
/*     return FLT_MAX; */
/*   } */

/*   return (*ppc->pdVariable); */

/* } */


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

/*
%typemap(in) void *pvVariable
{

  double dTemp = PyFloat_AsDouble($input);

  double *dOutput = (double*)malloc(sizeof(double));

  *dOutput = dTemp;

  $1 = (double*)dOutput;

}


%typemap(in) void *pvInput
{

  double dTemp = PyFloat_AsDouble($input);

  $1 = (double*)&dTemp;

  printf("Got a void pointer 1: %f\n",(double)(dTemp));


}

*/

extern double GetVariableValue(struct simobj_PerfectClamp *ppc);
/* extern double GetLevel(struct simobj_PerfectClamp *ppc); */

extern int PerfectClampAddVariable(struct simobj_PerfectClamp * ppc, void *pvVariable);
extern int PerfectClampAddVariable_Double(struct simobj_PerfectClamp * ppc, double *pdVariable);

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
