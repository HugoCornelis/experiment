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


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "experiment/perfectclamp.h"


/// 
/// \arg ppc perfect clamper.
/// \arg pv pointer to the variable to be clamped, assumed is double *.
/// 
/// \return int
/// 
///	success of operation.
/// 
/// \brief Clamp the given variable during the simulation.
/// 

int
PerfectClampAddVariable
(struct simobj_PerfectClamp * ppc, void *pvVariable)
{
    //- set default result: ok

    int iResult = 1;

    if (ppc->iClampsActive >= 1)
    {
	return(0);
    }

    //- set next variable

    ppc->pdVariable = (double *)pvVariable;

    ppc->iClampsActive++;

    //- return result

    return(iResult);
}


/// 
/// \arg ppc perfect clamper.
/// \arg pd pointer to the variable to be clamped, assumed is double *.
/// 
/// \return int
/// 
///	success of operation.
/// 
/// \brief Clamp the given variable during the simulation.
/// 

int
PerfectClampAddVariable_Double
(struct simobj_PerfectClamp * ppc, double *pdVariable)
{
    //- set default result: ok

    int iResult = 1;

    if (ppc->iClampsActive >= 1)
    {
	return(0);
    }

    //- set next variable

    ppc->pdVariable = pdVariable;

    ppc->iClampsActive++;

    //- return result

    return(iResult);
}


/// 
/// \arg ppc perfect clamper.
/// 
/// \return int
/// 
///	success of operation.
/// 
/// \brief Release memory occupied by the perfect clamper.
/// 

int PerfectClampFinish(struct simobj_PerfectClamp * ppc)
{
    //- set default result: ok

    int iResult = 1;

    //- close files

    if (ppc->pfile)
    {
	fclose(ppc->pfile);
    }

    if (ppc->pcFilename)
    {
	free(ppc->pcFilename);
    }

    //- free all allocated memory

    free(ppc->pcName);

    free(ppc);

    //- return result

    return(iResult);
}


/// 
/// \arg ppc perfect clamper.
/// 
/// \return int
/// 
///	success of operation.
/// 
/// \brief Initiate the perfect clamper.
/// 

int PerfectClampInitiate(struct simobj_PerfectClamp * ppc)
{
    //- set default result: ok

    int iResult = 1;

    //- return result

    return(iResult);
}


/// 
/// \arg pcName name of this object.
/// 
/// \return struct simobj_PerfectClamp *
/// 
///	perfect clamper, NULL for failure.
/// 
/// \brief perfect clamper.
/// 

struct simobj_PerfectClamp * PerfectClampNew(char *pcName)
{
    //- set default result: failure

    struct simobj_PerfectClamp * ppcResult = NULL;

    //- allocate perfect clamper

    ppcResult = (struct simobj_PerfectClamp *) calloc(1, sizeof(struct simobj_PerfectClamp));

    if (!ppcResult)
    {
	return(NULL);
    }

    //- set name

    ppcResult->pcName = calloc(1 + strlen(pcName), sizeof(char));

    strcpy(ppcResult->pcName, pcName);

    //- return result

    return(ppcResult);
}


/// 
/// \arg ppc perfect clamper.
/// \arg dCommand command level, ignored if a filename is given.
/// \arg pcFilename filename, file contains sequence of command levels to be applied during the simulation.
/// 
/// \return int
/// 
///	success of operation.
/// 
/// \brief set operation fields of the perfect clamper.
/// 

int
PerfectClampSetFields
(struct simobj_PerfectClamp * ppc,
 double dCommand,
 char *pcFilename)
{
    //- set default result: ok

    int iResult = 1;

    //- set fields

    ppc->dCommand = dCommand;

    //- if a filename has been given

    if (pcFilename)
    {
	//- allocate filename

	ppc->pcFilename = calloc(1 + strlen(pcFilename), sizeof(char));

	strcpy(ppc->pcFilename, pcFilename);

	//- open file

	/// \note I need the qualification service overhere.

	ppc->pfile = fopen(pcFilename, "r");

	if (!ppc->pfile)
	{
	    fprintf(stderr, "warning: PerfectClampSetFields() cannot open command file\n");

	    PerfectClampFinish(ppc);

	    return(0);
	}

	//- file stays open during simulation

	/// \note we could also cache the file in memory, depending on an option or so.
    }

    //- return result

    return(iResult);
}


/// 
/// \arg ppc perfect clamper.
/// \arg dTime current simulation time.
/// 
/// \return int
/// 
///	success of operation.
/// 
/// \brief Compute new command levels, set new variable values.
/// 
/// \note 
/// 
///	Old values are overwritten.
/// 

int PerfectClampSingleStep(struct simobj_PerfectClamp * ppc, double dTime)
{
    //- set default result: ok

    int iResult = 1;

    //- if running with a file

    if (ppc->pfile)
    {
	//- read a command levels from the file

	char pcStep[100];

	char pcCommand[100];

	int iEOF = fscanf(ppc->pfile, " %[^:]: %s", pcStep, pcCommand);

	ppc->dCommand = strtod(pcCommand, (char **)NULL);

	//- if end of file

	if (iEOF == EOF)
	{
	    //- rewind the file

	    if (fseek(ppc->pfile, 0L, SEEK_SET) != 0)
	    {
		fprintf(stderr, "warning: PerfectClampSingleStep() cannot rewind the command file\n");
	    }
	}
	else if (iEOF != 2)
	{
	    fprintf(stderr, "warning: PerfectClampSingleStep() read %i values from the command file (instead of 2)\n", iEOF);
	}

	//- set the output

	*ppc->pdVariable = ppc->dCommand;
    }
    else
    {
	//- set the output

	*ppc->pdVariable = ppc->dCommand;
    }

    //- return result

    return(iResult);
}


