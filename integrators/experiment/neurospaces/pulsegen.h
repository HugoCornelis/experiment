#ifndef CHEMESIS3_NEUROSPACES_CHEMESIS3_H
#define CHEMESIS3_NEUROSPACES_CHEMESIS3_H


#define PRE_PROTO_TRAVERSAL 1

#include <neurospaces/neurospaces.h>



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



//s translator specific service data

struct Chemesis3TranslationServiceData
{
    //m neurospaces

    struct Neurospaces *pneuro;

    //m root symbol in neurospaces

    struct PidinStack *ppistRoot;

    struct symtab_HSolveListElement *phsleRoot;

    //m the model to be solved by this instantiation

    int iModel;
};


int Chemesis3ConnectDistributor(struct simobj_Chemesis3 *pch3, struct EventDistributor *ped, struct SolverRegistry *psr, struct ProjectionQuery *ppq);

int Chemesis3ConnectQueuer(struct simobj_Chemesis3 *pch3, struct EventQueuer *peq, struct SolverRegistry *psr, struct ProjectionQuery *ppq);

int Chemesis3Construct(struct simobj_Chemesis3 *pch3, void *pvNeurospaces, char *pcModel, void *pvEventDistributor, void *pvEventQueuer);

int Chemesis3NeurospacesKinetics2Chemesis3(struct Chemesis3TranslationService *pcts);


#endif


