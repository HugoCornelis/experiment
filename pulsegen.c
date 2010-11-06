//-------------------------------------------------------------------
/*!
*  \file pulsegen.c
*  \author Mando Rodriguez
*
*  Original code by M. Nelson Caltech 4/89 
*  This file holds the implementation for PulseGen which has been
*  ported from GENESIS 2.3. Taken from the development notes:
* 
* This device can generate a variety of pulse patterns:  single pulses, double
* pulses (each with its own level and width), and pulse trains (of single
* pulses or pairs).  It can be triggered, gated, or allowed to free run.
* 
* In combination with a spikegen object, it can be used to generate bursts of
* spikes for use as synaptic input.  (See Scripts/neuron/inputs.g.)
* 
* The following diagram illustrates the relationships between the fields:
* 
*                       +-----------------+ level1
*   trigger             |                 |
*      |                |                 |        +-------------+ level2
*      |                |                 |        |             |
*      V                |                 |        |             |
*      +--- baselevel --+                 +--------+             +----
* 
*      <---- delay1 ----><---- width1 ---->         <-- width2 -->
*                        <--------- delay2 -------->
* If delay2 <= width1, then level2 starts imediately after the end of the
* pulse at level1.  This means that if delay2, width2 and level2 are not set
* and remain at their default values of 0, there will be only one type of
* pulse.  It will repeat after a time delay1 after the end of the pulse
* (free run mode) or a time delay1 after it is triggered (trigger or gate
* mode).
 */
//-------------------------------------------------------------------
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "experiment/pulsegen.h"


//-------------------------------------------------------------------
/*!
 * \fn int PulseGenSetFields(struct simobj_PulseGen *ppg,double dLevel1, double dWidth1,
 double dDelay1,double dLevel2,double dWidth2,double dDelay2,int iTriggerMode,double *pdPulseOut)
* \param ppg A pointer to a PulseGen struct.
* \param dLevel1 level of pulse1
* \param dWidth width of pulse1
* \param dDelay delay of pulse1
* \param dLevel2 level of pulse2
* \param dWidth2 width of pulse2
* \param dDelay2 delay of pulse2
* \param iTriggerMode Trigger mode for the pulsegen: 0 - free run, 1 - ext. trig, 2 - ext gate   
* \return 0 on error, 1 on success.
*
* Function sets the fields for an allocated PulseGen struct.
 */
//-------------------------------------------------------------------
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
 int iTriggerMode
)
{

  if(!ppg)
  {
    fprintf(stderr,"%s","No PulseGen Simulation Object Allocated.\n");
    return 0;
  }

  ppg->dLevel1 = dLevel1;
  ppg->dWidth1 = dWidth1;
  ppg->dDelay1 = dDelay1;
  ppg->dLevel2 = dLevel2;
  ppg->dWidth2 = dWidth2;
  ppg->dDelay2 = dDelay2;
  ppg->dBaseLevel = dBaseLevel;
  ppg->iTriggerMode = iTriggerMode;

  return 1;
}




//-------------------------------------------------------------------
/*!
 * \fn int PulseGenSingleStep(struct simobj_PulseGen *ppg, double dTime)
 * \return -1 on error, 1 on success, 0 for no operation.
 *
 * Processes a single time step for a pulsegen object.
 */
//-------------------------------------------------------------------
int PulseGenSingleStep(struct simobj_PulseGen *ppg, double dTime)
{

  int iInput;
  int iPulseNum;
  double dTCycle; 
  double dTT = 0.0;

  
  if( ppg->dWidth1 > (ppg->dDelay2 + ppg->dWidth2) )
  {
    dTCycle = ppg->dDelay1 + ppg->dWidth1;
  }
  else
  {
    dTCycle = ppg->dDelay1 + ppg->dDelay2 + ppg->dWidth2;
  }


  iInput = 0;

  /*INPUT*/
  /*
   * For the script PulseGenDemo.g the only message we
   * handle is input, which is simply setting the iInput value to 1.
   * The other three types of messages will need more work:
   * LEVEL, WIDTH, DELAY 
   */
  //iInput = 1;

  //
  // Here we perform actions based on the trigger mode.
  // Note: some of the comments here are lifted straight
  // from the GENESIS 2 file.
  //
  if( ppg->iTriggerMode == FREE_RUN )
  {

    dTT = fmod(dTime,dTCycle);

  }
  else if( ppg->iTriggerMode == EXT_GATE )
  {

    /* gate low */
    if( iInput == 0 )
    {
      /* output = baselevel */
      dTT = dTCycle;
    }
    else
    {
      /* gate high */

      if(ppg->iPreviousInput == 0)
      {
	/* low -> high */
	ppg->dTriggerTime = dTime;
      }

      dTT = fmod(dTime - ppg->dTriggerTime,dTCycle);
    }

    ppg->iPreviousInput = iInput;

  }
  else if( ppg->iTriggerMode == EXT_TRIG )
  {
    /* trig low */
    if( iInput == 0 )
    {
      /* never triggered */
      if(ppg->dTriggerTime < 0.0)
      {
	/* output = baselevel */
	dTT = dTCycle;
      }
      else
      {
	dTT = dTime - ppg->dTriggerTime;
      }


    }
    else /* trig high */ 
    {
      /* low -> high */
      if( ppg->iPreviousInput == 0 )
      {
	ppg->dTriggerTime = dTime;
      }

      /* don't use fmod here */
      dTT = dTime - ppg->dTriggerTime;

    }

    ppg->iPreviousInput = iInput;

  }


  //
  // Post processing, we set our output 
  // from the results of the step.
  //

  if(!ppg->pdPulseOut)
    return -1;

  if( dTT < ppg->dDelay1 || dTT >= dTCycle )
  {

    (*ppg->pdPulseOut) = ppg->dBaseLevel;

  }
  else if( dTT < (ppg->dDelay1 + ppg->dWidth1) )
  {
    
    (*ppg->pdPulseOut) = ppg->dLevel1;

  }
  else if( dTT < (ppg->dDelay1 + ppg->dDelay2) )
  {

    (*ppg->pdPulseOut) = ppg->dBaseLevel;

  }
  else
  {

    (*ppg->pdPulseOut) = ppg->dLevel2;
    
  }


  return 1;

}



//-------------------------------------------------------------------
/*!
 * \fn struct simobj_PulseGen * PulseGenNew(char *pcName)
 * \param pcName The name for this pulsegen object.
 * \return A pointer to a struct on success, NULL on error.
 *
 */
//-------------------------------------------------------------------
struct simobj_PulseGen * PulseGenNew(char *pcName)
{

  struct simobj_PulseGen *ppgResult = NULL;

  ppgResult = (struct simobj_PulseGen*)calloc(1,sizeof(struct simobj_PulseGen));

  if(!ppgResult)
  {
    
    return NULL;

  }

  ppgResult->pcName = strdup(pcName);

  ppgResult->dLevel1 = DBL_MAX;
  ppgResult->dWidth1 = DBL_MAX;
  ppgResult->dDelay1 = DBL_MAX;
  ppgResult->dLevel2 = DBL_MAX;
  ppgResult->dWidth2 = DBL_MAX;
  ppgResult->dDelay2 = DBL_MAX;
  ppgResult->dBaseLevel = DBL_MAX;
  ppgResult->iTriggerMode = -1;
  ppgResult->pdPulseOut = NULL;
  //
  // This sets the new pulsegen to a "never triggered" 
  // state. Not sure how to handle it recieving a message
  // to "trigger" it.
  //
  ppgResult->dTriggerTime = -1;

  return ppgResult;

}

//-------------------------------------------------------------------
/*!
 *  \fn int PulseGenReset(struct simobj_PulseGen *ppg)
 *  \param ppg A pointer to a PulseGen simulation object.
 *  \return 0 on error, 1 on success.
 *
 *  Sets values for the pulse generator to their deault state.
 */
//-------------------------------------------------------------------
int PulseGenReset(struct simobj_PulseGen *ppg)
{

  if(!ppg)
  {
    fprintf(stderr,"%s","Error: No PulseGen simulation object allocated, can't reset.\n");
    return 0;
  }

  ppg->iPreviousInput = 0.0;
  ppg->iTriggerMode = -1;
  
  if(!ppg->pdPulseOut)
  {
    fprintf(stderr,"%s","Error: No PulseOut defined, can't reset output.\n");

    // We return 0 here even to the other variables have been correctly
    // reset.
    return 0;
  }
  else
  {
    (*ppg->pdPulseOut) = ppg->dBaseLevel;
  }

  return 1;

}


//-------------------------------------------------------------------
/*!
 *  \fn int PulseGenFinish(struct simobj_PulseGen *ppg)
 *  \param ppg A pointer to a PulseGen simulation object.
 *  \return 0 on error, 1 on success.
 *
 *  Frees an allocated PulseGen object.
 */
//-------------------------------------------------------------------
int PulseGenFinish(struct simobj_PulseGen *ppg)
{

  if(!ppg)
  {
    return 0;
  }

  free(ppg);

  return 1;

}


//-------------------------------------------------------------------
/*!
 *  \fn int PulseGenAddInput(struct simobj_PulseGen *ppg,void pvInput)
 *  \param ppg A pointer to a PulseGen simulation object.
 *  \return 0 on no operation, 1 on success.
 *
 *  Adds an Input to a Pulsegen
 */
//-------------------------------------------------------------------
int PulseGenAddInput(struct simobj_PulseGen *ppg, void *pvInput)
{

  if(!ppg)
  {
    return 0;
  }

  if(ppg->pdPulseIn != NULL)
  {
    return 0;
  }

  ppg->pdPulseIn = (double *)pvInput;

  return 1;

}


//-------------------------------------------------------------------
/*!
 *  \fn int PulseGenAddInput(struct simobj_PulseGen *ppg,void pvInput)
 *  \param ppg A pointer to a PulseGen simulation object.
 *  \return 0 on no operation, 1 on success.
 *
 *  Adds an Output to a Pulsegen
 */
//-------------------------------------------------------------------
int PulseGenAddVariable(struct simobj_PulseGen *ppg, void *pvOutput)
{

  if(!ppg)
  {
    return 0;
  }

  if(ppg->pdPulseOut != NULL)
  {
    return 0;
  }

  ppg->pdPulseOut = (double *)pvOutput;

  return 1;

}

