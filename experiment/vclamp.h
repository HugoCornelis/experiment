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


#ifndef HECCER_VCLAMP_H
#define HECCER_VCLAMP_H


#include <stdio.h>


struct VClamp
{
    /// a string identifier, for what it is worth

    char *pcName;

    /// number of clamped variables

    int iClampsActive;

    /// input, solved variables

    double *pdVoltage;

    double *pdInjector;

    /// command current

    double dInjected;

    /// intial resistance and capacitance

    double dC;
    double dR;

    /// resulting command voltage

    double dCommand;

    /// gain and time constants

    /// \note setfield ^ gain 0.50 tau_i 0.02 tau_d 0.005 saturation 999.0

    double dGain;
    double dTau_i;
    double dTau_d;
    double dSaturation;

    /// solved state variables

    double dE;
    double dEIntegral;
    double dEDerivative;
    double dEPrevious;

    /// previous time of calculations

    double dPreviousTime;

    /// final output

    double dOutput;

};


int
VClampAddVariable
(struct VClamp * pvc, double *pdVoltage, double *pdInjector);

int VClampSingleStep(struct VClamp * pvc, double dTime);

int VClampFinish(struct VClamp * pvc);

int VClampInitiate(struct VClamp * pvc);

struct VClamp * VClampNew(char *pcName);

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
 double dSaturation);


#endif


