%module pulsegen_base	

/***************************************************
* Start C code block
***************************************************/
%{
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../../experiment/pulsegen.h"

// This is just an example function
int fact(int nonnegative)
{

  return (nonnegative - 1);

}
%}

/***************************************************
* End C code block
***************************************************/



/* commented out for now
%typemap(in) double {

  $1 = PyFloat_AsDouble($input);
  printf("Checked a double : %f\n",  (double)$1);
}



%typemap(in) char *pcName {
  printf("Got a name!\n");

  $1 = $input;
}
*/

%typemap(in) void *pvOutput
{

  double dTemp = PyFloat_AsDouble($input);

  $1 = (double*)&dTemp;

  printf("Got a void pointer: %f\n",(double)(dTemp));


}


%typemap(in) void *pvInput
{

  double dTemp = PyFloat_AsDouble($input);

  $1 = (double*)&dTemp;

  printf("Got a void pointer: %f\n",(double)(dTemp));


}

%typemap(out) void *pvInput
{

  $result = PyFloat_FromDouble($1);

  printf("Outputting a void pointer: %f\n",(double)($1));


}


%typemap(out) void *pvInput
{

  $result = PyFloat_FromDouble($1);

  printf("Outputting a void pointer: %f\n",(double)($1));


}

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
extern int PulseGenAddInput(struct simobj_PulseGen *ppg, void *pvInput);
extern int PulseGenAddVariable(struct simobj_PulseGen *ppg, void *pvOutput);


/* Commented out for now
%typemap(in) int {
  $1 = PyInt_AsLong($input);
  printf("Received an integer : %d\n",  (int)$1);
  $1+= 100;
}

%typemap(out) int {
  $result = PyInt_FromLong($1);
  printf("Outputting an integer : %d\n",  (int)$1);
}
*/

%inline %{
extern int fact(int nonnegative);
%}


/*------------------------------------------------------------------------
* Grab the original header file so SWIG can import the prototypes and
* create low level data classes out of the member structs. Apparently this
* line must be at the end of the file otherwise SWIG will ignore my typemaps.
*------------------------------------------------------------------------*/
%include "../../../../../experiment/pulsegen.h"
