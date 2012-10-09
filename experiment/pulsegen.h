//------------------------------------------------------------------
/*!
 *  \file nsintegrator.h
 *  \author Mando Rodriguez
 *
 *  This file holds the declarations for the pulsegen algorithm.
 */
//-------------------------------------------------------------------

#ifndef SIMULATIONOBJECTS_PULSEGEN__H
#define SIMULATIONOBJECTS_PULSEGEN__H



/*
 * Trigger mode defines taken from GENESIS.
 */
#define FREE_RUN	0
#define EXT_TRIG	1
#define EXT_GATE	2


struct simobj_PulseGen
{
    char *pcName;

    double dLevel1;
    double dWidth1;
    double dDelay1;

    double dLevel2;
    double dWidth2;
    double dDelay2;

    double dBaseLevel;

    double dTriggerTime;
    int iTriggerMode;

    int iPreviousInput;

    /// An input value referenced via pointer. 
  
    double *pdPulseIn;

    /// solved variables

    double *pdPulseOut;
};


struct simobj_PulseGen * PulseGenNew(char *pcName);

int PulseGenFinish(struct simobj_PulseGen *ppg);

int PulseGenAddInput(struct simobj_PulseGen *ppg, void *pvInput);

int PulseGenAddVariable(struct simobj_PulseGen *ppg, void *pvOutput);

int PulseGenAddVariable_Double(struct simobj_PulseGen *ppg, double *pdOutput);

int PulseGenReset(struct simobj_PulseGen *ppg);

int PulseGenSingleStep(struct simobj_PulseGen *ppg, double dTime);

int PulseGenSetFields
(
    struct simobj_PulseGen *ppg,
    double dLevel1,
    double dWidth1,
    double dDelay1,
    double dLevel2,
    double dWidth2,
    double dDelay2,
    double dBaseLevel,
    int iTriggerMode);


#endif
