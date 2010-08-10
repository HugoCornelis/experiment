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


struct PerfectClamp
{
    /// a string identifier, for what it is worth

    char *pcName;

    /// number of clamped variables

    int iClampsActive;

    /// input, solved variable

    double *pdVoltage;

    /// command voltage

    double dCommand;

    /// filename with command voltages

    char *pcFilename;

    FILE *pfile;

};


int
PerfectClampAddVariable
(struct PerfectClamp * ppc, void *pvVoltage);

int PerfectClampSingleStep(struct PerfectClamp * ppc, double dTime);

int PerfectClampFinish(struct PerfectClamp * ppc);

int PerfectClampInitiate(struct PerfectClamp * ppc);

struct PerfectClamp * PerfectClampNew(char *pcName);

int
PerfectClampSetFields
(struct PerfectClamp * ppc,
 double dCommand,
 char *pcFilename);


#endif


