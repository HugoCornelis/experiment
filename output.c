//
// Experiment : a compartmental solver that implements efficient Crank-Nicolson
// integration for neuronal models.
//

//////////////////////////////////////////////////////////////////////////////
//'
//' Experiment : testbed C implementation
//'
//' Copyright (C) 2006-2008 Hugo Cornelis
//'
//' functional ideas ..	Hugo Cornelis, hugo.cornelis@gmail.com
//'
//' coding ............	Hugo Cornelis, hugo.cornelis@gmail.com
//'
//////////////////////////////////////////////////////////////////////////////


#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "experiment/output.h"


#define ALLOCATE_VARIABLES 10000


/// 
/// \arg pog output generator.
/// \arg pcName name of the variable.
/// \arg pdVariable pointer to the variable.
/// 
/// \return int
/// 
///	success of operation.
/// 
/// \brief Schedule a variable for output.
/// 
/// Error: the impediment of this experiment
/// 

int
OutputGeneratorAddVariable
(struct OutputGenerator * pog, char *pcName, double *pdVariable)
{
    //- set default result : ok

    int iResult = 1;

    if (pog->iVariablesActive >= ALLOCATE_VARIABLES)
    {
	return(0);
    }

    //- set next variable

    pog->ppdVariables[pog->iVariablesActive] = pdVariable;

    pog->iVariablesActive++;

    //- return result

    return(iResult);
}


/// 
/// \arg pog output generator.
/// \arg pc annotation for current values.
/// 
/// \return int
/// 
///	success of operation.
/// 
/// \brief Output all variable values, with the given annotation.
/// 

int OutputGeneratorAnnotatedStep(struct OutputGenerator * pog, char * pc)
{
    //- set default result : ok

    int iResult = 1;

    //- if we are not at the correct resolution time step

    if (pog->iResolution > 0)
    {
	pog->iResolutionStep--;

	if (pog->iResolutionStep > 0)
	{
	    //- nothing to output, return success

	    return(1);
	}

	pog->iResolutionStep = pog->iResolution;
    }

    //- default: we assume no output

    int iOutput = 1;

    int iMaxLengthVariables = 0;

    int iLengthVariables = 0;

    char *pcVariables = (char *) calloc(1, sizeof(char));

    pcVariables[0] = '\0';

    //- write out all the variables

    int i;

    for (i = 0 ; i < pog->iVariablesActive ; i++)
    {
	//- if the variable differs from the base value

	if (*pog->ppdVariables[i] != pog->dBase)
	{
#define iSingleVariableSize 100

	    char pcVariable[iSingleVariableSize] = "";

	    //- dynamically resize the output buffer for new variables

	    if (iMaxLengthVariables < iLengthVariables + iSingleVariableSize)
	    {
		iMaxLengthVariables += 10000;

		pcVariables = (char *) realloc(pcVariables, iMaxLengthVariables * sizeof(char));
	    }

	    //- when the no time option is in effect

	    if (pog->iEmitTime == 0 && i == 0)
	    {
		//- don't emit the white space prefix for the first entry

		iLengthVariables += sprintf(pcVariable, &pog->pcFormat[1], *pog->ppdVariables[i]);
	    }

	    //- if emitting time or for any entry other than the first

	    else
	    {
		//- emit the variable prefixed with a space

		iLengthVariables += sprintf(pcVariable, pog->pcFormat, *pog->ppdVariables[i]);
	    }

	    strcat(pcVariables, pcVariable);

	    //- register we have output

	    iOutput = 1;
	}
    }

    //- if there was any output

    if (iOutput)
    {
/* 	//- create the annotation */

/* 	if (pc) */
/* 	{ */
/* 	    /// \todo choose between yaml style and xplot style using options */

/* 	    fprintf(pog->pfileOutput, "%s", pc); */

/* 	    fprintf(pog->pfileOutput, "%s:", pc); */
/* 	} */

	//- write the output to the file

        if (pog->iEmitTime == 0)
	{
	    fprintf(pog->pfileOutput, "%s\n", pcVariables);
	}
	else
	{
	    fprintf(pog->pfileOutput, "%s%s\n", pc ? pc : "", pcVariables);	  
	}
    }

    free(pcVariables);

    //- return result

    return(iResult);
}


/// 
/// \arg pog output generator.
/// 
/// \return int
/// 
///	success of operation.
/// 
/// \brief Close files, destruct the output generator.
/// 

int OutputGeneratorFinish(struct OutputGenerator * pog)
{
    //- set default result : ok

    int iResult = 1;

    //- if file

    if (pog->pfileOutput)
    {
	//- close file

	if (fclose(pog->pfileOutput) == EOF)
	{
	    iResult = 0;
	}
    }

    //- free allocated memory

    free(pog->ppdVariables);

    free(pog->pcFilename);

    /// \note prevent accidental misuse

    pog->iVariablesAllocated = -9238;

    free(pog);

    //- return result

    return(iResult);
}


/// 
/// \arg pog output generator.
/// 
/// \return int
/// 
///	success of operation.
/// 
/// \brief Flush files.
/// 

int OutputGeneratorFlush(struct OutputGenerator * pog)
{
    //- set default result : ok

    int iResult = 1;

    //- if file

    if (pog->pfileOutput)
    {
	//- flush file buffers to disk

	if (fflush(pog->pfileOutput) == EOF)
	{
	    iResult = 0;
	}
    }

    //- return result

    return(iResult);
}


/// 
/// \arg pog output generator.
/// 
/// \return int
/// 
///	success of operation.
/// 
/// \brief Open files.
/// 

int OutputGeneratorInitiate(struct OutputGenerator * pog)
{
    //- set default result : ok

    int iResult = 1;

/*     //- if file not opened yet */

/*     if (!pog->pfileOutput) */
    {
	//- open file

	pog->pfileOutput = fopen(pog->pcFilename, "w");

	if (!pog->pfileOutput)
	{
	    return(0);
	}
    }

    //- return result

    return(iResult);
}


/// 
/// \arg pcFilename file name to write output to.
/// 
/// \return struct OutputGenerator *
/// 
///	Output generator, NULL for failure.
/// 
/// \brief Output generator.
/// 

struct OutputGenerator * OutputGeneratorNew(char *pcFilename)
{
    //- set default result : failure

    struct OutputGenerator * pogResult = NULL;

    //- allocate output generator

    pogResult = (struct OutputGenerator *)calloc(1, sizeof(struct OutputGenerator));

    if (!pogResult)
    {
	return(NULL);
    }

    //- allocate output variables

    pogResult->ppdVariables = (double **)calloc(ALLOCATE_VARIABLES, sizeof(double *));

    pogResult->iVariablesAllocated = ALLOCATE_VARIABLES;

    if (!pogResult->ppdVariables)
    {
	free(pogResult);

	return(NULL);
    }

    //- allocate filename

    pogResult->pcFilename = calloc(1 + strlen(pcFilename), sizeof(char));

    if (!pogResult->pcFilename)
    {
	free(pogResult->ppdVariables);

	free(pogResult);

	return(NULL);
    }

    strcpy(pogResult->pcFilename, pcFilename);

    //- initialize base value: DBL_MAX

    pogResult->dBase = DBL_MAX;

    //- initialize format

    pogResult->pcFormat = " %g";

    //- initialize time related variables

    pogResult->dTime = 0.0;

    pogResult->dStep = 0.0;

    //- initialize: non-steps mode

    pogResult->iSteps = 0;

    //- default: we emit time steps

    pogResult->iEmitTime = 1;

    //- return result

    return(pogResult);
}


/// 
/// \arg pog output generator.
/// \arg pcFormat new output printf type format.
/// 
/// \return int
/// 
///	success of operation.
/// 
/// \brief Set a new output format, see printf for details.
/// 

int OutputGeneratorSetFormat(struct OutputGenerator * pog, char *pcFormat)
{
    //- set default result : ok

    int iResult = 1;

    //- set format

    pog->pcFormat = pcFormat;

    //- return result

    return(iResult);
}


/// 
/// \arg pog output generator.
/// \arg iSteps 1 to turn on steps mode, 0 otherwise.
/// 
/// \return int
/// 
///	success of operation.
/// 
/// \brief Turn on / off steps mode.
/// 

int OutputGeneratorSetSteps(struct OutputGenerator * pog, int iSteps)
{
    //- set default result : ok

    int iResult = 1;

    //- set format

    pog->iSteps = iSteps;

    //- return result

    return(iResult);
}


/// 
/// \arg pog output generator.
/// \arg dTime simulation time.
/// 
/// \return int
/// 
///	success of operation.
/// 
/// \brief Output all variable values, at the given time.
///
/// \details This gets remapped to OutputGeneratorAnnotatedStep(),
/// with the time as annotation.
/// 
/// \todo Note that DES EventDistributorSend() calls the function
/// OutputGeneratorTimedStep() with an extra argument, namely iTarget.
/// See the corresponding comments in EventDistributorSend() and
/// OutputGeneratorTimedStep().
/// 

int OutputGeneratorTimedStep(struct OutputGenerator * pog, double dTime)
{
    //- set default result : ok

    int iResult = 1;

    if (pog->dStep != 0.0)
    {
	while (pog->dTime < dTime)
	{
	    //- update the simulation time

	    pog->dTime += pog->dStep;

	    //- create the annotation

	    char pc[100];

	    sprintf(pc, "%g", pog->dTime);

	    //- output an annotated step

	    iResult = OutputGeneratorAnnotatedStep(pog, pc);

	    if (iResult != 1)
	    {
		return(iResult);
	    }
	}
    }
    else
    {
	//- update the simulation time

	pog->dTime = dTime;

	//- create the annotation

	char pc[100];

	sprintf(pc, "%g", pog->dTime);

	//- output an annotated step

	iResult = OutputGeneratorAnnotatedStep(pog, pc);
    }

    //- return result

    return(iResult);
}



/// 
/// \arg pog output generator.
/// \arg pcLine A string with the line to write
/// 
/// \return int
/// 
///	success of operation.
/// 
/// \brief Writes a single line out to the open file descriptor.
///
/// \details Just does a direct print to the file output object if it exists
/// 
/// 
int OutputGeneratorWriteLine(struct OutputGenerator * pog, char *pcLine)
{

  if(!pog || !pcLine)
  {
    return 0;
  }

  if(!pog->pfileOutput)
  {
    return 0;
  }

  fprintf(pog->pfileOutput, "%s\n", pcLine);

  return 1;

}
