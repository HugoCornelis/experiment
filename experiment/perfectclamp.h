//
// Heccer : a compartmental solver that implements efficient Crank-Nicolson
// integration for neuronal models.
//

//////////////////////////////////////////////////////////////////////////////
//'
//' Heccer : testbed C implementation
//'
//' Copyright (C) 2006-2008 Hugo Cornelis
//'
//' functional ideas ..	Hugo Cornelis, hugo.cornelis@gmail.com
//'
//' coding ............	Hugo Cornelis, hugo.cornelis@gmail.com
//'
//////////////////////////////////////////////////////////////////////////////


#ifndef HECCER_PERFECTCLAMP_H
#define HECCER_PERFECTCLAMP_H


#include <stdio.h>


struct simobj_PerfectClamp
{
    /// a string identifier, for what it is worth

    char *pcName;

    /// number of clamped variables

    int iClampsActive;

    /// output, solved variable

    double *pdVariable;

    /// command level

    double dCommand;

    /// filename with command levels

    char *pcFilename;

    FILE *pfile;

};


int
PerfectClampAddVariable
(struct simobj_PerfectClamp * ppc, void *pvVariable);

int
PerfectClampAddVariable_Double
(struct simobj_PerfectClamp * ppc, double *pdVariable);

int PerfectClampSingleStep(struct simobj_PerfectClamp * ppc, double dTime);

int PerfectClampFinish(struct simobj_PerfectClamp * ppc);

int PerfectClampInitiate(struct simobj_PerfectClamp * ppc);

struct simobj_PerfectClamp * PerfectClampNew(char *pcName);

int
PerfectClampSetFields
(struct simobj_PerfectClamp * ppc,
 double dCommand,
 char *pcFilename);


#endif


