

#include <stdio.h>
#include <stdlib.h>

#include <neurospaces/pidinstack.h>
#include <neurospaces/treespacetraversal.h>

#include "experiment/addressing.h"
#include "experiment/service.h"


/********************** prototypes *********************************************/

static int pulsegen_processor(struct TreespaceTraversal *ptstr, void *pvUserdata);


/********************** end prototypes *****************************************/


/********************** structs ************************************************/
struct pulsegen_info
{

  struct simobj_PulseGen *ppg;

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
  
  double *pdPulseIn;

  double *pdPulseOut;

}
/********************** end structs *******************************************/

static
int
pulsegen_processor(struct TreespaceTraversal *ptstr, void *pvUserdata)
{
    //- set default result : ok

    int iResult = TSTR_PROCESSOR_SUCCESS;

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
  
    double *pdPulseIn;

    double *pdPulseOut;

    //- get pointer to solver

    struct pulsegen_info *ppgi = (struct pulsegen_info *)pvUserdata;

    struct simobj_Pulsegen *ppg = ppgi->ppg;

    //- get actual symbol

    int iType = TstrGetActualType(ptstr);

    struct symtab_HSolveListElement *phsle = (struct symtab_HSolveListElement *)TstrGetActual(ptstr);


    //- If a pulsegen we populate it

    if (instanceof_pulse_gen(iType))
    {


      dLevel1 = SymbolParameterResolveValue(phsle, ptstr->ppist, "LEVEL1");

      if (dLevel1 == DBL_MAX)
      {
	iResult = TSTR_PROCESSOR_ABORT;
      }

      dWidth1 = SymbolParameterResolveValue(phsle, ptstr->ppist, "WIDTH1");

      if (dWidth1 == DBL_MAX)
      {
	iResult = TSTR_PROCESSOR_ABORT;
      }

      dDelay1 = SymbolParameterResolveValue(phsle, ptstr->ppist, "DELAY1");

      if (dDelay1 == DBL_MAX)
      {
	iResult = TSTR_PROCESSOR_ABORT;
      }

      dLevel2 = SymbolParameterResolveValue(phsle, ptstr->ppist, "LEVEL2");

      if (dLevel2 == DBL_MAX)
      {
	iResult = TSTR_PROCESSOR_ABORT;
      }

      dWidth2 = SymbolParameterResolveValue(phsle, ptstr->ppist, "WIDTH2");

      if (dWidth1 == DBL_MAX)
      {
	iResult = TSTR_PROCESSOR_ABORT;
      }

      dDelay2 = SymbolParameterResolveValue(phsle, ptstr->ppist, "DELAY2");

      if (dDelay1 == DBL_MAX)
      {
	iResult = TSTR_PROCESSOR_ABORT;
      }


      dBaseLevel = SymbolParameterResolveValue(phsle, ptstr->ppist, "BASELEVEL");

      if (dBaseLevel == DBL_MAX)
      {
	iResult = TSTR_PROCESSOR_ABORT;
      }


      dTriggerMode = SymbolParameterResolveValue(phsle, ptstr->ppist, "TRIGMODE");

      if (dTriggerMode == DBL_MAX)
      {
	iResult = TSTR_PROCESSOR_ABORT;
      }

      
      if (iResult == TSTR_PROCESSOR_ABORT)
      {
/* 	char pc[1000]; */

/* 	PidinStackString(ptstr->ppist, pc, sizeof(pc)); */

      }


    } //- End of isinstance_pulse_gen


    //- return result

    return(iResult);
}




/*******************************************************************************
 *
********************************************************************************/
int PulseGenConstruct(struct simobj_PulseGen *pch3, void *pvNeurospaces, char *pcModel, void *pvEventDistributor, void *pvEventQueuer)
{
    //- the event_distributor is an event_distributor

    struct EventDistributor *ped = (struct EventDistributor *)pvEventDistributor;

    pch3->ped = ped;

    //- the event_distributor is an event_distributor

    struct EventQueuer *peq = (struct EventQueuer *)pvEventQueuer;

    pch3->peq = peq;

    //- the modeling service is neurospaces

    struct Neurospaces *pneuro = (struct Neurospaces *)pvNeurospaces;

    //- set default result : ok

    int iResult = 1;

    //- register the modelname in the chemesis3

    pch3->pcName = strdup(pcModel);

    //- lookup the model

    struct PidinStack *ppistModel = PidinStackParse(pcModel);

    if (!ppistModel)
    {
	fprintf(stderr, "PulseGenConstruct: cannot parse model name %s\n", pcModel);

	return(FALSE);
    }

    //- allocate the translation service structures

    struct PulseGenTranslationServiceData *pctsd
	= (struct PulseGenTranslationServiceData *)calloc(1, sizeof(struct PulseGenTranslationServiceData));

    struct PulseGenTranslationService *pcts
	= (struct PulseGenTranslationService *)calloc(1, sizeof(struct PulseGenTranslationService));

    pcts->pctsd = pctsd;

    pcts->pctsd->pneuro = pneuro;

    //- allocate pidin stack pointing to root

    struct PidinStack *ppistRoot = PidinStackCalloc();

    if (!ppistRoot)
    {
	fprintf(stderr, "PulseGenConstruct: cannot allocate a root context for %s\n", pcModel);

	return(FALSE);
    }

    PidinStackSetRooted(ppistRoot);

    /// \note gosh, I had to do the same hack when integrating neurospaces
    /// \note with genesis2/hsolve.

    struct ParserContext *pacRoot = pneuro->pacRootContext;

    struct ImportedFile *pifRoot
	= ParserContextGetImportedFile(pacRoot);

    /// \note depending on how the linking is done, there can be multiple
    /// \note instances of neurospaces around.  The following is a hack to
    /// \note enforce the singleton (a bit)

/*     fprintf(stdout, "PulseGenConstruct(): root import is %p\n", ImportedFileGetRootImport()); */

    ImportedFileSetRootImport(pifRoot);

/*     fprintf(stdout, "PulseGenConstruct(): root import is %p\n", ImportedFileGetRootImport()); */

    //- update caches

    struct symtab_HSolveListElement *phsleRoot
	= PidinStackLookupTopSymbol(ppistRoot);

    pctsd->ppistRoot = ppistRoot;

    pctsd->phsleRoot = phsleRoot;

    //- set model

    struct symtab_HSolveListElement *phsleModel
	= PidinStackLookupTopSymbol(ppistModel);

    pctsd->iModel = PidinStackToSerial(ppistModel);

    if (pctsd->iModel == INT_MAX)
    {
	/// \note don't care about memory leak right now, consider this
	/// \note right now as a fatal crash

	fprintf(stderr, "PulseGenConstruct: cannot find model %s\n", pcModel);

	return(FALSE);
    }

    if (!PulseGenNeurospacesKinetics2PulseGen(pcts))
    {
	/// \note don't care about memory leak right now, consider this
	/// \note right now as a fatal crash

	fprintf(stderr, "PulseGenConstruct: compartment initialization failed for %s\n", pcModel);

	return(FALSE);
    }

    //- set service structures

    pch3->pcts = pcts;

    //- initialize the range of the intermediary, for the addressing
    //- function

    /// \note note: closed interval, would probably be better to use a halve
    /// \note open interval.

    /// \todo PulseGenConstruct() should not touch inter.
    /// \todo move or copy the field iSerialStart to pctsd

    pch3->iSerialStart = ADDRESSING_NEUROSPACES_2_CHEMESIS3(pctsd->iModel);

    pch3->iSerialEnd = ADDRESSING_NEUROSPACES_2_CHEMESIS3(pctsd->iModel + SymbolGetPrincipalNumOfSuccessors(phsleModel));

    //- free allocated data

    PidinStackFree(ppistModel);

    //- return result

    return(iResult);
}

