#ifndef EXPERIMENT_SERVICE_H
#define EXPERIMENT_SERVICE_H


struct simobj_PulseGen;

struct PulseGenTranslationService;

struct PulseGenTranslationServiceData;


typedef int (*ComponentInspector)(struct simobj_PulseGen *pch3, struct PulseGenTranslationService *cpts);

struct PulseGenTranslationService
{
    /// service specific data

    struct PulseGenTranslationServiceData *pctsd;

    /// \todo guess need to map this to the biological level as defined by
    /// \todo Neurospaces, not done yet.

    /// \todo additionally need to map this to the 'spatial and temporal
    /// \todo accuracy level'.  Not sure how to do that yet.

    /// initializes chemesis3 solver

    ComponentInspector kinetic_inspector;

/*     /// initializes mechanism intermediary */

/*     ComponentInspector mechanisms_inspector; */
};


#endif


