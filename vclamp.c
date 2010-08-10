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


#include "experiment/vclamp.h"


/// 
/// \arg pvc voltage clamper.
/// \arg pvVoltage pointer to the voltage variable, assumed is double *
/// \arg pvInjector pointer to the variable for injected current.
/// 
/// \return int
/// 
///	success of operation.
/// 
/// \brief Clamp the given voltage variable, using the given injector.
/// 

int
VClampAddVariable
(struct VClamp * pvc, void *pvVoltage, void *pvInjector)
{
    //- set default result: ok

    int iResult = 1;

    if (pvc->iClampsActive >= 1)
    {
	return(0);
    }

    //- set next variable

    pvc->pdVoltage = (double *)pvVoltage;

    pvc->pdInjector = (double *)pvInjector;

    pvc->iClampsActive++;

    //- return result

    return(iResult);
}


/// 
/// \arg pvc voltage clamper.
/// 
/// \return int
/// 
///	success of operation.
/// 
/// \brief Free the voltage clamper.
/// 

int VClampFinish(struct VClamp * pvc)
{
    //- set default result: ok

    int iResult = 1;

    //- free all allocated memory

    free(pvc);

    //- return result

    return(iResult);
}


/// 
/// \arg pvc voltage clamper.
/// 
/// \return int
/// 
///	success of operation.
/// 
/// \brief Initiate the voltage clamper.
/// 

int VClampInitiate(struct VClamp * pvc)
{
    //- set default result: ok

    int iResult = 1;

    //- just clear out some varialbes...

    /// \todo should initialize from a stream as heccer does.

    pvc->dEIntegral = 0;
    pvc->dEPrevious = 0;
    pvc->dOutput = 0;

    //- return result

    return(iResult);
}


/// 
/// \arg pcName name of this object.
/// 
/// \return struct VClamp *
/// 
///	voltage clamper, NULL for failure.
/// 
/// \brief voltage clamper.
/// 

struct VClamp * VClampNew(char *pcName)
{
    //- set default result: failure

    struct VClamp * pvcResult = NULL;

    //- allocate voltage clamper

    pvcResult = (struct VClamp *)calloc(1, sizeof(struct VClamp));

    if (!pvcResult)
    {
	return(NULL);
    }

    //- set name

    pvcResult->pcName = calloc(1 + strlen(pcName), sizeof(char));

    strcpy(pvcResult->pcName, pcName);

    //- return result

    return(pvcResult);
}


/// 
/// \arg dInjected injected current.
/// \arg dC parallel initial capacitance.
/// \arg dR initial resistance.
/// \arg dCommand_init initial command voltage.
/// \arg dGain gain value.
/// \arg dTau_i integrating time constant.
/// \arg dTau_d derivative time constant.
/// \arg dSaturation saturation value.
/// 
/// \return int
/// 
///	success of operation.
/// 
/// \brief set operation fields of voltage clamper.
/// 
/// \note 
/// 
///	The initial command voltage should probably have a separate
///	setter method.
/// 

int
VClampSetFields
(struct VClamp * pvc,
 double dInjected,
 double dC,
 double dR,
 double dCommand_init,
 double dGain,
 double dTau_i,
 double dTau_d,
 double dSaturation)
{
    //- set default result: ok

    int iResult = 1;

    //- set fields

    pvc->dInjected = dInjected;

    pvc->dC = dC;
    pvc->dR = dR;
    pvc->dCommand = dCommand_init;

    pvc->dGain = dGain;
    pvc->dTau_i = dTau_i;
    pvc->dTau_d = dTau_d;
    pvc->dSaturation = dSaturation;

    //- return result

    return(iResult);
}


/// 
/// \arg pvc voltage clamper.
/// \arg dTime current simulation time.
/// 
/// \return int
/// 
///	success of operation.
/// 
/// \brief Compute new currents to correct voltages.
/// 
/// \note 
/// 
///	Old current values are overwritten.
/// 

int VClampSingleStep(struct VClamp * pvc, double dTime)
{
    //- set default result: ok

    int iResult = 1;

    //- calculate the time step

    double dStep = dTime - pvc->dPreviousTime;

    //- exponential euler integration for the RC circuit

    double dA = pvc->dInjected / pvc->dC;

    double dB = 1 / (pvc->dR * pvc->dC);

    double dD = exp( - dB * dStep);

    pvc->dCommand = (pvc->dCommand * dD + (dA / dB) * (1 - dD));

    //- first preserve the previous value

    pvc->dEPrevious = pvc->dE;

    pvc->dE = pvc->dCommand - *pvc->pdVoltage;

    pvc->dEDerivative = (pvc->dE - pvc->dEPrevious) / dStep;

    pvc->dEIntegral += 0.5 * (pvc->dE + pvc->dEPrevious) * dStep;

    //- compute output

    pvc->dOutput
	= (pvc->dGain *
	   (pvc->dE
	    + pvc->dTau_d * pvc->dEDerivative
	    + 1 / pvc->dTau_i * pvc->dEIntegral));

    //- apply saturation

    if (pvc->dOutput > pvc->dSaturation)
    {
	pvc->dOutput = pvc->dSaturation;

	pvc->dEIntegral -= 0.5 * (pvc->dE + pvc->dEPrevious) * dStep;
    }
    else if (pvc->dOutput < - pvc->dSaturation)
    {
	pvc->dOutput = - pvc->dSaturation;

	pvc->dEIntegral -= 0.5 * (pvc->dE + pvc->dEPrevious) * dStep;
    }

    //- set the output

    *pvc->pdInjector = pvc->dOutput;

    //- register the current simulation time for the next time we are scheduled

    pvc->dPreviousTime = dTime;

    //- return result

    return(iResult);
}


