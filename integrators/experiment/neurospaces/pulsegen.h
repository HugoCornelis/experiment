#ifndef PULSE_GEN_NEUROSPACES_PULSE_GEN_H
#define PULSE_GEN_NEUROSPACES_PULSE_GEN_H


#define PRE_PROTO_TRAVERSAL 1

#include <neurospaces/neurospaces.h>







//s translator specific service data

struct PulseGenTranslationServiceData
{
    //m neurospaces

    struct Neurospaces *pneuro;

    //m root symbol in neurospaces

    struct PidinStack *ppistRoot;

    struct symtab_HSolveListElement *phsleRoot;

    //m the model to be solved by this instantiation

    int iModel;
};


int PulseGenConnectDistributor(struct simobj_PulseGen *pch3, struct EventDistributor *ped, struct SolverRegistry *psr, struct ProjectionQuery *ppq);

int PulseGenConnectQueuer(struct simobj_PulseGen *pch3, struct EventQueuer *peq, struct SolverRegistry *psr, struct ProjectionQuery *ppq);

int PulseGenConstruct(struct simobj_PulseGen *pch3, void *pvNeurospaces, char *pcModel, void *pvEventDistributor, void *pvEventQueuer);

int PulseGenNeurospacesKinetics2PulseGen(struct PulseGenTranslationService *pcts);


#endif


