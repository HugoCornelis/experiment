%module pulsegen_base	


//Import our structs and make data objects out of them
%include "../../../../../experiment/pulsegen.h"

//%include "cpointer.i"

//%pointer_functions(int,intp);

//%pointer_functions(double,doublep);


//--------------------------------------------------------------------------------
%{
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../../experiment/pulsegen.h"

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

int fact(int nonnegative)
{

  return (nonnegative - 1);

}
%}
//--------------------------------------------------------------------------------


%typemap(in) void *pvInput
{
  
  printf("Recieved a void %f\n",(double)(*$input));


}

%typemap(out) void *pvInput
{
  
  printf("Recieved a void %f\n",(double)(*$input));


}


%typemap(in) void * pvOutput
{
  
  printf("Recieved a void %f\n",(double)(*$input));


}


%typemap(out) void * pvOutput
{
  
  printf("Recieved a void %f\n",(double)(*$input));


}




%typemap(out) void * pvOutput
{
  
  printf("Recieved a void %f\n",(double)(*$input));


}


%typemap(in) void * pvOutput {
  printf("Received an pvOutput : %f\n",  (double)*$1);
}

// Wrap these functions
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



%typemap(in) int {
  $1 = PyInt_AsLong($input);
  printf("Received an integer : %d\n",  (int)$1);
}

%typemap(out) int {
  $result = PyInt_FromLong($1);
}
%inline %{
extern int fact(int nonnegative);
%}
