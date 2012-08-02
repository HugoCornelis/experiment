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


#ifndef HECCER_OUTPUTGENERATOR_H
#define HECCER_OUTPUTGENERATOR_H


#include <stdio.h>


struct OutputGenerator
{
    /// number of variables

    int iVariablesAllocated;

    int iVariablesActive;

    /// all variables

    double **ppdVariables;

    /// base value: only values different from this are recorded

    double dBase;

    /// output file stream

    char *pcFilename;

    FILE *pfileOutput;

    /// steps mode

    int iSteps;

    /// output format

    char *pcFormat;

    /// output resolution: modulo factor

    int iResolution;

    /// output resolution: current step

    int iResolutionStep;

    int iNoTimeStep;

/*     /// simulation time */

/*     double dSimulationTime; */

};

int
OutputGeneratorAddVariable
(struct OutputGenerator * pog, char *pcName, double *pdVariable);

int OutputGeneratorAnnotatedStep(struct OutputGenerator * pog, char * pc);

int OutputGeneratorFinish(struct OutputGenerator * pog);

int OutputGeneratorFlush(struct OutputGenerator * pog);

int OutputGeneratorInitiate(struct OutputGenerator * pog);

struct OutputGenerator * OutputGeneratorNew(char *pcFilename);

int OutputGeneratorSetFormat(struct OutputGenerator * pog, char *pcFormat);

int OutputGeneratorSetSteps(struct OutputGenerator * pog, int iSteps);

int OutputGeneratorTimedStep(struct OutputGenerator * pog, double dTime);


#endif


