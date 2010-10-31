%module pulsegen_base	

%include "../../../../../experiment/pulsegen.h"

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



// Wrap these functions
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
int PulseGenSingleStep(struct simobj_PulseGen *ppg, double dTime);;
struct simobj_PulseGen * PulseGenNew(char *pcName);
int PulseGenReset(struct simobj_PulseGen *ppg);
int PulseGenFinish(struct simobj_PulseGen *ppg);
int PulseGenAddInput(struct simobj_PulseGen *ppg, void *pvInput);
int PulseGenAddVariable(struct simobj_PulseGen *ppg, void *pvOutput);

%include pulsegen.i
