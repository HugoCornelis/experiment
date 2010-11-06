%module pulsegen_base	

# Some detailed swig information for handling pointers
#- http://embedded.eecs.berkeley.edu/Alumni/pinhong/scriptEDA/pyTypemapFAQ.html#overcast


%{
/* Include directives necessary for your following definitions
    should be here. */
/* #include "pulsegen.c" */
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../../experiment/pulsegen.h"
%}



#Import our structs and make data objects out of them
%include "../../../../../experiment/pulsegen.h"

%typemap (in) double*
{
  if (PyCObject_Check($input))
  {

    $1 = PyCObject_AsVoidPtr($input)

  }
}

// Wrap these functions
%{
extern struct simobj_PulseGen * PulseGenNew(char *pcName);
extern int PulseGenSetFields(
	struct simobj_PulseGen *ppg,
	double dLevel1,
 	double dWidth1,
 	double dDelay1,
 	double dLevel2,
 	double dWidth2,
 	double dDelay2,
 	double dBaseLevel,
 	int iTriggerMode
	);
extern int PulseGenSingleStep(struct simobj_PulseGen *ppg, double dTime);
extern struct simobj_PulseGen * PulseGenNew(char *pcName);
extern int PulseGenReset(struct simobj_PulseGen *ppg);
extern int PulseGenFinish(struct simobj_PulseGen *ppg);
%}

%typemap (in) void*
{
  if (PyCObject_Check($input))
  {

    $1 = PyCObject_AsVoidPtr($input)

  }
}

int PulseGenAddInput(struct simobj_PulseGen *ppg, void *pvInput);
int PulseGenAddVariable(struct simobj_PulseGen *ppg, void *pvOutput);

%include pulsegen.i
