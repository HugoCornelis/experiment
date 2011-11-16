

#include <stdio.h>
#include <stdlib.h>

#include <neurospaces/pidinstack.h>
#include <neurospaces/treespacetraversal.h>

#include "chemesis3/addressing.h"
#include "chemesis3/neurospaces/chemesis3.h"
#include "chemesis3/service.h"


struct kinetics_info
{
    //m the solver to initialize

    struct simobj_Chemesis3 *pch3;

    //m number of pools found

    int iPools;

    //m number of reactions found

    int iReactions;

    //m number of diffusions found

    int iDiffusions;

};


static int chemesis3_setup_kinetics(struct simobj_Chemesis3 *pch3, struct Chemesis3TranslationService *pts);

static int solver_complete_indexes(struct simobj_Chemesis3 *pch3);

static
int
solver_counter(struct TreespaceTraversal *ptstr, void *pvUserdata);

static
int
solver_index_recorder(struct TreespaceTraversal *ptstr, void *pvUserdata);

static int solver_pool_serial_2_index(struct simobj_Chemesis3 *pch3, int iSerial);

static struct ch3_pool * solver_pool_serial_2_pool(struct simobj_Chemesis3 *pch3, int iSerial);

static
int
solver_processor(struct TreespaceTraversal *ptstr, void *pvUserdata);

static int solver_serials_2_indexes(struct simobj_Chemesis3 *pch3);


static int solver_complete_indexes(struct simobj_Chemesis3 *pch3)
{
    //- set default result : ok

    int iResult = TRUE;

    //- loop over all reactions

    int iReaction;

    for (iReaction = 0 ; iReaction < pch3->iReactions ; iReaction++)
    {
	struct ch3_reaction *preaction = &pch3->preaction[iReaction];

	//- count all pools of this reaction

	{
	    //- loop over all products

	    int iProduct;

	    for (iProduct = 0 ; iProduct < preaction->iProducts ; iProduct++)
	    {
		//- count this reaction for this pool

		int iPoolIndex = preaction->piProducts[iProduct];

		struct ch3_pool *ppoolAttached = &pch3->ppool[iPoolIndex];

		if (ppoolAttached->iConserve)
		{
		}
		else
		{
		    ppoolAttached->iReactions++;
		}
	    }

	    //- loop over all substrates

	    int iSubstrate;

	    for (iSubstrate = 0 ; iSubstrate < preaction->iSubstrates ; iSubstrate++)
	    {
		//- count this reaction for this pool

		int iPoolIndex = preaction->piSubstrates[iSubstrate];

		struct ch3_pool *ppoolAttached = &pch3->ppool[iPoolIndex];

		if (ppoolAttached->iConserve)
		{
		}
		else
		{
		    ppoolAttached->iReactions++;
		}
	    }
	}
    }

    //- loop over all pools

    int iPool;

    for (iPool = 0 ; iPool < pch3->iPools ; iPool++)
    {
	//- allocate reaction link memory

	struct ch3_pool *ppool = &pch3->ppool[iPool];

	if (ppool->iReactions)
	{
	    ppool->piReactions = (int *)calloc(ppool->iReactions + 100, sizeof(int));

	    ppool->piReactionFlags = (int *)calloc(ppool->iReactions + 100, sizeof(int));
	}
	else
	{
	    ppool->piReactions = NULL;

	    ppool->piReactionFlags = NULL;
	}

	//- and reset the reaction link counter

	ppool->iReactions = 0;
    }

    //- loop over all reactions

    for (iReaction = 0 ; iReaction < pch3->iReactions ; iReaction++)
    {
	struct ch3_reaction *preaction = &pch3->preaction[iReaction];

	{
	    //- loop over all products

	    int iProduct;

	    for (iProduct = 0 ; iProduct < preaction->iProducts ; iProduct++)
	    {
		//- register this reaction for this pool

		int iPoolIndex = preaction->piProducts[iProduct];

		struct ch3_pool *ppoolAttached = &pch3->ppool[iPoolIndex];

		if (ppoolAttached->iConserve)
		{
		}
		else
		{
		    ppoolAttached->piReactions[ppoolAttached->iReactions] = iReaction;

		    //- flag this pool as a product for this reaction

		    ppoolAttached->piReactionFlags[ppoolAttached->iReactions] = 1;

		    //- and increase counter

		    ppoolAttached->iReactions++;
		}
	    }

	    //- loop over all substrates

	    int iSubstrate;

	    for (iSubstrate = 0 ; iSubstrate < preaction->iSubstrates ; iSubstrate++)
	    {
		//- register this reaction for this pool

		int iPoolIndex = preaction->piSubstrates[iSubstrate];

		struct ch3_pool *ppoolAttached = &pch3->ppool[iPoolIndex];

		if (ppoolAttached->iConserve)
		{
		}
		else
		{
		    ppoolAttached->piReactions[ppoolAttached->iReactions] = iReaction;

		    //- flag this pool as a product for this reaction

		    ppoolAttached->piReactionFlags[ppoolAttached->iReactions] = -1;

		    //- and increase counter

		    ppoolAttached->iReactions++;
		}
	    }
	}
    }

    //- return result

    return(iResult);
}


static
int
solver_counter(struct TreespaceTraversal *ptstr, void *pvUserdata)
{
    //- set default result : ok

    int iResult = TSTR_PROCESSOR_SUCCESS;

    //- get pointer to solver

    struct kinetics_info *pki = (struct kinetics_info *)pvUserdata;

    struct simobj_Chemesis3 *pch3 = pki->pch3;

    //- get actual symbol

    int iType = TstrGetActualType(ptstr);

    struct symtab_HSolveListElement *phsle = (struct symtab_HSolveListElement *)TstrGetActual(ptstr);

    //- count pools

    if (subsetof_pool(iType))
    {
	pki->iPools++;
    }

    //- count reactions

    //- if a reaction

    int iIsReaction = 0;

    if (subsetof_reaction(iType))
    {
	iIsReaction = 1;
    }

    // \todo implement a proper reaction type in the model-container

    if (!iIsReaction
	&& subsetof_group(iType))
    {
	double dBackwardRate
	    = SymbolParameterResolveValue(phsle, ptstr->ppist, "BACKWARD_RATE");

	double dForwardRate
	    = SymbolParameterResolveValue(phsle, ptstr->ppist, "FORWARD_RATE");

	if (dBackwardRate != DBL_MAX
	    && dForwardRate != DBL_MAX)
	{
	    iIsReaction = 1;
	}
    }

    if (iIsReaction)
    {
	pki->iReactions++;
    }

    //- count diffusions

    // \todo implement a proper kinetics type in the model-container

/*     if (subsetof_segment(iType)) */
    {
	struct symtab_Parameters *ppar
	    = SymbolFindParameter(phsle, ptstr->ppist, "diffusion_constant");

	if (ppar)
	{
	    pki->iDiffusions++;
	}
    }

    //- return result

    return(iResult);
}


static
int
solver_index_recorder(struct TreespaceTraversal *ptstr, void *pvUserdata)
{
    //- set default result : ok

    int iResult = TSTR_PROCESSOR_SUCCESS;

    //- get pointer to solver

    struct kinetics_info *pki = (struct kinetics_info *)pvUserdata;

    struct simobj_Chemesis3 *pch3 = pki->pch3;

    //- get actual symbol

    int iType = TstrGetActualType(ptstr);

    struct symtab_HSolveListElement *phsle = (struct symtab_HSolveListElement *)TstrGetActual(ptstr);

    //- if a pool

    if (subsetof_pool(iType))
    {
	//- register current symbol

	int iPool = pki->iPools;

	//- register type

	pch3->ppool[iPool].mc.iType = MATH_TYPE_Pool;

	//- register identification

	/// \note note: assumes pp define CHEMESIS3_SOURCE_NEUROSPACES

	int iSerial = PidinStackToSerial(ptstr->ppist);

	iSerial = ADDRESSING_NEUROSPACES_2_CHEMESIS3(iSerial);

	pch3->ppool[iPool].mc.iSerial = iSerial;

#ifdef CHEMESIS3_SOURCE_TYPING

	double dModelSourceType
	    = SymbolParameterResolveValue(phsle, ptstr->ppist, "MODEL_SOURCE_TYPE");

	if (dModelSourceType != DBL_MAX)
	{
	    pch3->ppool[iPool].mc.iModelSourceType = dModelSourceType;
	}
	else
	{
	    pch3->ppool[iPool].mc.iModelSourceType = -1;
	}

#endif

	//- increment number of solved pools

	pki->iPools++;

    }

    //- if a reaction

    int iIsReaction = 0;

    if (subsetof_reaction(iType))
    {
	iIsReaction = 1;
    }

    // \todo implement a proper reaction type in the model-container

    if (!iIsReaction
	&& subsetof_group(iType))
    {
	double dBackwardRate
	    = SymbolParameterResolveValue(phsle, ptstr->ppist, "BACKWARD_RATE");

	double dForwardRate
	    = SymbolParameterResolveValue(phsle, ptstr->ppist, "FORWARD_RATE");

	if (dBackwardRate != DBL_MAX
	    && dForwardRate != DBL_MAX)
	{
	    iIsReaction = 1;
	}
    }

    if (iIsReaction)
    {
	//- register current symbol

	int iReaction = pki->iReactions;

	//- register type

	pch3->preaction[iReaction].mc.iType = MATH_TYPE_Reaction;

	//- register identification

	/// \note note: assumes pp define CHEMESIS3_SOURCE_NEUROSPACES

	int iSerial = PidinStackToSerial(ptstr->ppist);

	iSerial = ADDRESSING_NEUROSPACES_2_CHEMESIS3(iSerial);

	pch3->preaction[iReaction].mc.iSerial = iSerial;

#ifdef CHEMESIS3_SOURCE_TYPING

	double dModelSourceType
	    = SymbolParameterResolveValue(phsle, ptstr->ppist, "MODEL_SOURCE_TYPE");

	if (dModelSourceType != DBL_MAX)
	{
	    pch3->preaction[iReaction].mc.iModelSourceType = dModelSourceType;
	}
	else
	{
	    pch3->preaction[iReaction].mc.iModelSourceType = -1;
	}

#endif

	//- increment number of solved reactions

	pki->iReactions++;

    }

    // \todo implement a proper kinetics type in the model-container

/*     if (subsetof_segment(iType)) */
    {
	struct symtab_Parameters *ppar
	    = SymbolFindParameter(phsle, ptstr->ppist, "diffusion_constant");

	if (ppar)
	{
	    //- register current symbol

	    int iDiffusion = pki->iDiffusions;

	    //- register type

	    pch3->pdiffusion[iDiffusion].mc.iType = MATH_TYPE_Diffusion;

	    //- register identification

	    /// \note note: assumes pp define CHEMESIS3_SOURCE_NEUROSPACES

	    int iSerial = PidinStackToSerial(ptstr->ppist);

	    iSerial = ADDRESSING_NEUROSPACES_2_CHEMESIS3(iSerial);

	    pch3->pdiffusion[iDiffusion].mc.iSerial = iSerial;

#ifdef CHEMESIS3_SOURCE_TYPING

	    double dModelSourceType
		= SymbolParameterResolveValue(phsle, ptstr->ppist, "MODEL_SOURCE_TYPE");

	    if (dModelSourceType != DBL_MAX)
	    {
		pch3->pdiffusion[iDiffusion].mc.iModelSourceType = dModelSourceType;
	    }
	    else
	    {
		pch3->pdiffusion[iDiffusion].mc.iModelSourceType = -1;
	    }

#endif

	    //- increment number of solved diffusions

	    pki->iDiffusions++;

	}
    }

    //- return result

    return(iResult);
}


static
int
solver_processor(struct TreespaceTraversal *ptstr, void *pvUserdata)
{
    //- set default result : ok

    int iResult = TSTR_PROCESSOR_SUCCESS;

    //- get pointer to solver

    struct kinetics_info *pki = (struct kinetics_info *)pvUserdata;

    struct simobj_Chemesis3 *pch3 = pki->pch3;

    //- get actual symbol

    int iType = TstrGetActualType(ptstr);

    struct symtab_HSolveListElement *phsle = (struct symtab_HSolveListElement *)TstrGetActual(ptstr);

    //- if a pool

    if (subsetof_pool(iType))
    {
	//- register current symbol

	int iPool = pki->iPools;

	//- register parameters

	/// \todo check for error returns, abort traversal if necessary

	double dVolume
	    = SymbolParameterResolveValue(phsle, ptstr->ppist, "VOLUME");

	if (dVolume == DBL_MAX)
	{
	    iResult = TSTR_PROCESSOR_ABORT;
	}

	double dConcentrationInit
	    = SymbolParameterResolveValue(phsle, ptstr->ppist, "concen_init");

	if (dConcentrationInit == DBL_MAX)
	{
	    iResult = TSTR_PROCESSOR_ABORT;
	}

	double dConcentrationTotal
	    = SymbolParameterResolveValue(phsle, ptstr->ppist, "concen_total");

/* 	if (dConcentrationTotal == DBL_MAX) */
/* 	{ */
/* 	    iResult = TSTR_PROCESSOR_ABORT; */
/* 	} */

	pch3->ppool[iPool].dVolume = dVolume;
	pch3->ppool[iPool].dConcentrationInit = dConcentrationInit;
	pch3->ppool[iPool].dConcentrationTotal = dConcentrationTotal;

	if (dConcentrationTotal == DBL_MAX)
	{
	    pch3->ppool[iPool].dUnits = 1e-3;

	    pch3->ppool[iPool].iConserve = 0;
	}
	else
	{
	    pch3->ppool[iPool].dUnits = 0;

	    pch3->ppool[iPool].iConserve = 1;
	}

	//- if this pool have an attached pool

	struct PidinStack *ppistPoolAttached
	    = SymbolResolveInput(phsle, ptstr->ppist, "concen", 0);

	if (ppistPoolAttached)
	{
	    //- count attached pools

	    int i;

	    for (i = 0 ; ppistPoolAttached ; i++)
	    {
		PidinStackFree(ppistPoolAttached);

		ppistPoolAttached
		    = SymbolResolveInput(phsle, ptstr->ppist, "concen", i);
	    }

	    //- allocate memory for indexing

	    pch3->ppool[iPool].piPools = (int *)calloc(i - 1 + 100, sizeof(int));

	    pch3->ppool[iPool].piPoolsFlags = (int *)calloc(i - 1 + 100, sizeof(int));

	    //- loop over all attached pools

	    ppistPoolAttached
		= SymbolResolveInput(phsle, ptstr->ppist, "concen", 0);

	    for (i = 0 ; ppistPoolAttached ; i++)
	    {
		PidinStackFree(ppistPoolAttached);

		//- get context of the attached pool

		ppistPoolAttached
		    = SymbolResolveInput(phsle, ptstr->ppist, "concen", i);

		if (ppistPoolAttached)
		{
		    //- fill in the serial of the attached pool

		    int iPoolAttached = PidinStackToSerial(ppistPoolAttached);

		    pch3->ppool[iPool].piPools[i] = ADDRESSING_NEUROSPACES_2_CHEMESIS3(iPoolAttached);

		    // \todo ask avrama for an example where this
		    // value is different, currently the '0' encocdes
		    // a 'CONC' message from a rxnpool to a
		    // conservepool.

		    pch3->ppool[iPool].piPoolsFlags[i] = 0;

		}
	    }

	    //- fill in the number of attached pools

	    pch3->ppool[iPool].iPools = i - 1;
	}
	else
	{
	    pch3->ppool[iPool].iPools = 0;
	}

	if (iResult == TSTR_PROCESSOR_ABORT)
	{
	    char pc[1000];

	    PidinStackString(ptstr->ppist, pc, sizeof(pc));

	    Chemesis3Error
		(pch3,
		 NULL,
		 "pool array translation failed at pool %s (pool %i, serial %i)\n",
		 pc,
		 pki->iPools + 1,
		 pch3->ppool[iPool].mc.iSerial);
	}

	//- increment number of solved pools

	pki->iPools++;

    }

    char *pcName = SymbolGetName(phsle);

    if (strcmp(pcName, "cabufrxn") == 0)
    {
	int iBreak = 1;
    }

    //- if a reaction

    int iIsReaction = 0;

    if (subsetof_reaction(iType))
    {
	iIsReaction = 1;
    }

    // \todo implement a proper reaction type in the model-container

    if (!iIsReaction
	&& subsetof_group(iType))
    {
	double dBackwardRate
	    = SymbolParameterResolveValue(phsle, ptstr->ppist, "BACKWARD_RATE");

	double dForwardRate
	    = SymbolParameterResolveValue(phsle, ptstr->ppist, "FORWARD_RATE");

	if (dBackwardRate != DBL_MAX
	    && dForwardRate != DBL_MAX)
	{
	    iIsReaction = 1;
	}
    }

    if (iIsReaction)
    {
	//- register current symbol

	int iReaction = pki->iReactions;

	//- register parameters

	/// \todo check for error returns, abort traversal if necessary

	double dBackwardRate
	    = SymbolParameterResolveValue(phsle, ptstr->ppist, "BACKWARD_RATE");

	if (dBackwardRate == DBL_MAX)
	{
	    iResult = TSTR_PROCESSOR_ABORT;
	}

	double dForwardRate
	    = SymbolParameterResolveValue(phsle, ptstr->ppist, "FORWARD_RATE");

	if (dForwardRate == DBL_MAX)
	{
	    iResult = TSTR_PROCESSOR_ABORT;
	}

	pch3->preaction[iReaction].dBackwardRate = dBackwardRate;
	pch3->preaction[iReaction].dBackwardSolved = 0;
	pch3->preaction[iReaction].dForwardRate = dForwardRate;
	pch3->preaction[iReaction].dForwardSolved = 0;

	//- if this reaction has products

	struct PidinStack *ppistProduct
	    = SymbolResolveTypedInput(phsle, ptstr->ppist, "concen", "product", 0);

	if (ppistProduct)
	{
	    //- count products

	    int i;

	    for (i = 0 ; ppistProduct ; i++)
	    {
		PidinStackFree(ppistProduct);

		ppistProduct
		    = SymbolResolveTypedInput(phsle, ptstr->ppist, "concen", "product", i);
	    }

	    //- allocate memory for indexing

	    pch3->preaction[iReaction].piProducts = (int *)calloc(i - 1 + 100, sizeof(int));

	    //- loop over all products

	    ppistProduct
		= SymbolResolveTypedInput(phsle, ptstr->ppist, "concen", "product", 0);

	    for (i = 0 ; ppistProduct ; i++)
	    {
		PidinStackFree(ppistProduct);

		//- get context of the product

		ppistProduct
		    = SymbolResolveTypedInput(phsle, ptstr->ppist, "concen", "product", i);

		if (ppistProduct)
		{
		    //- fill in the serial of the product

		    int iProduct = PidinStackToSerial(ppistProduct);

		    pch3->preaction[iReaction].piProducts[i] = ADDRESSING_NEUROSPACES_2_CHEMESIS3(iProduct);
		}
	    }

	    //- fill in the number of products

	    pch3->preaction[iReaction].iProducts = i - 1;
	}
	else
	{
	    pch3->preaction[iReaction].iProducts = 0;
	}

	//- if this reaction has substrates

	struct PidinStack *ppistSubstrate
	    = SymbolResolveTypedInput(phsle, ptstr->ppist, "concen", "substrate", 0);

	if (ppistSubstrate)
	{
	    //- count substrates

	    int i;

	    for (i = 0 ; ppistSubstrate ; i++)
	    {
		PidinStackFree(ppistSubstrate);

		ppistSubstrate
		    = SymbolResolveTypedInput(phsle, ptstr->ppist, "concen", "substrate", i);
	    }

	    //- allocate memory for indexing

	    pch3->preaction[iReaction].piSubstrates = (int *)calloc(i - 1 + 100, sizeof(int));

	    //- loop over all substrates

	    ppistSubstrate
		= SymbolResolveTypedInput(phsle, ptstr->ppist, "concen", "substrate", 0);

	    for (i = 0 ; ppistSubstrate ; i++)
	    {
		PidinStackFree(ppistSubstrate);

		//- get context of the substrate

		ppistSubstrate
		    = SymbolResolveTypedInput(phsle, ptstr->ppist, "concen", "substrate", i);

		if (ppistSubstrate)
		{
		    //- fill in the serial of the substrate

		    int iSubstrate = PidinStackToSerial(ppistSubstrate);

		    pch3->preaction[iReaction].piSubstrates[i] = ADDRESSING_NEUROSPACES_2_CHEMESIS3(iSubstrate);
		}
	    }

	    //- fill in the number of substrates

	    pch3->preaction[iReaction].iSubstrates = i - 1;
	}
	else
	{
	    pch3->preaction[iReaction].iSubstrates = 0;
	}

	if (iResult == TSTR_PROCESSOR_ABORT)
	{
	    char pc[1000];

	    PidinStackString(ptstr->ppist, pc, sizeof(pc));

	    Chemesis3Error
		(pch3,
		 NULL,
		 "reaction array translation failed at reaction %s (reaction %i, serial %i)\n",
		 pc,
		 pki->iReactions + 1,
		 pch3->preaction[iReaction].mc.iSerial);
	}

	//- increment number of solved reactions

	pki->iReactions++;

    }

    // \todo implement a proper kinetics type in the model-container

/*     if (subsetof_segment(iType)) */
    {
	struct symtab_Parameters *ppar
	    = SymbolFindParameter(phsle, ptstr->ppist, "diffusion_constant");

	if (ppar)
	{
	    //- register current symbol

	    int iDiffusion = pki->iDiffusions;

	    //- register parameters

	    /// \todo check for error returns, abort traversal if necessary

	    double dDiffusionConstant
		= SymbolParameterResolveValue(phsle, ptstr->ppist, "diffusion_constant");

	    if (dDiffusionConstant == DBL_MAX)
	    {
		iResult = TSTR_PROCESSOR_ABORT;
	    }

	    pch3->pdiffusion[iDiffusion].dDiffusionConstant = dDiffusionConstant;

	    pch3->pdiffusion[iDiffusion].dLength1 = 0;
	    pch3->pdiffusion[iDiffusion].dArea1 = 0;

	    pch3->pdiffusion[iDiffusion].ppool1 = NULL;

	    pch3->pdiffusion[iDiffusion].dLength2 = 0;
	    pch3->pdiffusion[iDiffusion].dArea2 = 0;

	    pch3->pdiffusion[iDiffusion].ppool2 = NULL;

	    pch3->pdiffusion[iDiffusion].dFlux2 = 0;
	    pch3->pdiffusion[iDiffusion].dFlux1 = 0;

	    pch3->pdiffusion[iDiffusion].dUnits = 1e-3;

	    //- find the morphological child

	    {
		struct PidinStack *ppistChild = PidinStackDuplicate(ptstr->ppist);

		PidinStackPushString(ppistChild, "Ca");

		PidinStackLookupTopSymbol(ppistChild);

		int iSerialPool1 = PidinStackToSerial(ppistChild);

		// \todo this implies that both pools are
		// already processed, this may not always be
		// the case.  A first traversal should process
		// all the pools, a second one connect them
		// via diffusion processing elements.

		struct ch3_pool *ppool1
		    = solver_pool_serial_2_pool(pch3, ADDRESSING_NEUROSPACES_2_CHEMESIS3(iSerialPool1));

		if (ppool1)
		{
		    pch3->pdiffusion[iDiffusion].ppool1 = ppool1;
		}
		else
		{
		    iResult = TSTR_PROCESSOR_ABORT;
		}

		//- free allocated memory

		PidinStackFree(ppistChild);
	    }

	    //- find the morphological parent

	    {
		struct symtab_Parameters *pparParent
		    = SymbolFindParameter(phsle, ptstr->ppist, "PARENT");

		if (pparParent)
		{
		    /// \todo I can just subtract the cell's segment ID ?

		    struct PidinStack *ppistParent
			= ParameterResolveToPidinStack(pparParent, ptstr->ppist);

		    if (ppistParent)
		    {
			// \todo make 'Ca' configurable, can be any identifier of a species

			PidinStackPushString(ppistParent, "Ca");

			struct symtab_HSolveListElement *phslePool2
			    = PidinStackLookupTopSymbol(ppistParent);

			int iSerialPool2 = PidinStackToSerial(ppistParent);

			// \todo this implies that both pools are
			// already processed, this may not always be
			// the case.  A first traversal should process
			// all the pools, a second one connect them
			// via diffusion processing elements.

			struct ch3_pool *ppool2
			    = solver_pool_serial_2_pool(pch3, ADDRESSING_NEUROSPACES_2_CHEMESIS3(iSerialPool2));

			if (ppool2)
			{
			    pch3->pdiffusion[iDiffusion].ppool2 = ppool2;
			}
			else
			{
			    iResult = TSTR_PROCESSOR_ABORT;
			}

/* 			int iParent = PidinStackToSerial(ppistParent); */

/* 			/// \todo check for error */

/* 			if (iParent != INT_MAX) */
/* 			{ */
/* 			    iParent = ADDRESSING_NEUROSPACES_2_CHEMESIS3(iParent); */

/* 			    pch3->ppool[iPool].iParent = iParent; */
/* 			} */
/* 			else */
/* 			{ */
/* 			    /// \note parent does not exist */

/* 			    iResult = TSTR_PROCESSOR_ABORT; */
/* 			} */

			//- free allocated memory

			PidinStackFree(ppistParent);
		    }
		    else
		    {
			iResult = TSTR_PROCESSOR_ABORT;
		    }
		}
		else
		{
		    iResult = TSTR_PROCESSOR_ABORT;
		}
	    }

	    if (iResult == TSTR_PROCESSOR_ABORT)
	    {
		char pc[1000];

		PidinStackString(ptstr->ppist, pc, sizeof(pc));

		Chemesis3Error
		    (pch3,
		     NULL,
		     "reaction array translation failed at kinetic %s (diffusion %i, serial %i)\n",
		     pc,
		     pki->iDiffusions + 1,
		     pch3->pdiffusion[iDiffusion].mc.iSerial);
	    }

	    pki->iDiffusions++;
	}
    }

    //- return result

    return(iResult);
}


static int solver_pool_serial_2_index(struct simobj_Chemesis3 *pch3, int iSerial)
{
    //- set default result: failure

    int iResult = -1;

    //- loop over all pools

    int iPool;

    for (iPool = 0 ; iPool < pch3->iPools ; iPool++)
    {
	//- if serials match

	struct ch3_pool *ppool = &pch3->ppool[iPool];

	if (ppool->mc.iSerial == iSerial)
	{
	    //- set result: current index

	    iResult = iPool;

	    break;
	}
    }

    //- return result

    return(iResult);
}


static struct ch3_pool * solver_pool_serial_2_pool(struct simobj_Chemesis3 *pch3, int iSerial)
{
    //- set default result: failure

    struct ch3_pool *ppoolResult = NULL;

    //- try to find the index

    int iPool = solver_pool_serial_2_index(pch3, iSerial);

    //- if found

    if (iPool != -1)
    {
	//- set result

	ppoolResult = &pch3->ppool[iPool];
    }

    //- return result

    return(ppoolResult);
}


static int solver_serials_2_indexes(struct simobj_Chemesis3 *pch3)
{
    //- set default result : ok

    int iResult = TRUE;

    //- loop over all pools

    int iPool;

    for (iPool = 0 ; iPool < pch3->iPools ; iPool++)
    {
	struct ch3_pool *ppool = &pch3->ppool[iPool];

	//- loop over all reactions attached to this pool

	int iReactionAttached;

	for (iReactionAttached = 0 ; iReactionAttached < ppool->iReactions ; iReactionAttached++)
	{
	    //- translate the serial to its index

	    int iReactionSerial = ppool->piReactions[iReactionAttached];

	    int iReactionIndex = solver_reaction_serial_2_index(iReactionSerial);

	    ppool->piReactions[iReactionAttached] = iReactionIndex;
	}

	//- loop over all pools attached to this pool

	int iPoolAttached;

	for (iPoolAttached = 0 ; iPoolAttached < ppool->iPools ; iPoolAttached++)
	{
	    //- translate the serial to its index

	    int iPoolSerial = ppool->piPools[iPoolAttached];

	    int iPoolIndex = solver_pool_serial_2_index(pch3, iPoolSerial);

	    ppool->piPools[iPoolAttached] = iPoolIndex;
	}

	//- loop over all diffusion connections attached to this pool

	int iDiffusionAttached;

	for (iDiffusionAttached = 0 ; iDiffusionAttached < ppool->iDiffusions ; iDiffusionAttached++)
	{
	}

    }

    //- loop over all reactions

    int iReaction;

    for (iReaction = 0 ; iReaction < pch3->iReactions ; iReaction++)
    {
	struct ch3_reaction *preaction = &pch3->preaction[iReaction];

	//- loop over all products

	int iProduct;

	for (iProduct = 0 ; iProduct < preaction->iProducts ; iProduct++)
	{
	    //- translate the serial to its index

	    int iPoolSerial = preaction->piProducts[iProduct];

	    int iPoolIndex = solver_pool_serial_2_index(pch3, iPoolSerial);

	    preaction->piProducts[iProduct] = iPoolIndex;
	}

	//- loop over all substrates

	int iSubstrate;

	for (iSubstrate = 0 ; iSubstrate < preaction->iSubstrates ; iSubstrate++)
	{
	    //- translate the serial to its index

	    int iPoolSerial = preaction->piSubstrates[iSubstrate];

	    int iPoolIndex = solver_pool_serial_2_index(pch3, iPoolSerial);

	    preaction->piSubstrates[iSubstrate] = iPoolIndex;
	}
    }

    //- return result

    return(iResult);
}


static int chemesis3_setup_kinetics(struct simobj_Chemesis3 *pch3, struct Chemesis3TranslationService *pcts)
{
    //- set default result : ok

    int iResult = TRUE;

    //- allocate pidin stack pointing to root

    struct PidinStack *ppistRoot = pcts->pctsd->ppistRoot;

    struct symtab_HSolveListElement *phsleRoot = pcts->pctsd->phsleRoot;

    //- get model context to solve

    struct PidinStack *ppistModel
	= SymbolPrincipalSerial2Context(phsleRoot, ppistRoot, pcts->pctsd->iModel);

    if (ppistModel)
    {
	struct kinetics_info ki =
	{
	    //m the solver to initialize

	    pch3,

	    //m number of pools found

	    0,

	    //m number of reactions found

	    0,

	    //m number of diffusions found

	    0,

	};

	//- lookup symbol

	struct symtab_HSolveListElement *phsleModel
	    = PidinStackLookupTopSymbol(ppistModel);

	//- loop over all symbols, count kinetic related ones

	struct TreespaceTraversal *ptstr
	    = TstrNew
	      (ppistModel,
	       NULL,
	       NULL,
	       solver_counter,
	       (void *)&ki,
	       NULL,
	       NULL);

	int iSuccess = TstrGo(ptstr, phsleModel);

	TstrDelete(ptstr);

	if (iSuccess == 1)
	{
	    if (ki.iPools != 0)
	    {
		//- allocate data structures

		pch3->ppool = (struct ch3_pool *)calloc(ki.iPools + 100, sizeof(struct ch3_pool));

		pch3->preaction = (struct ch3_reaction *)calloc(ki.iReactions + 100, sizeof(struct ch3_reaction));

		pch3->pdiffusion = (struct ch3_diffusion *)calloc(ki.iDiffusions + 100, sizeof(struct ch3_diffusion));

		if (pch3->ppool
		    && pch3->preaction
		    && pch3->pdiffusion)
		{
		    pch3->iDiffusions = ki.iDiffusions;
		    pch3->iPools = ki.iPools;
		    pch3->iReactions = ki.iReactions;

		    ki.iDiffusions = 0;
		    ki.iPools = 0;
		    ki.iReactions = 0;

		    struct TreespaceTraversal *ptstr
			= TstrNew
			  (ppistModel,
			   NULL,
			   NULL,
			   solver_index_recorder,
			   (void *)&ki,
			   NULL,
			   NULL);

		    int iSuccess = TstrGo(ptstr, phsleModel);

		    TstrDelete(ptstr);

		    if (iSuccess == 1)
		    {
			ki.iDiffusions = 0;
			ki.iPools = 0;
			ki.iReactions = 0;

			struct TreespaceTraversal *ptstr
			    = TstrNew
			      (ppistModel,
			       NULL,
			       NULL,
			       solver_processor,
			       (void *)&ki,
			       NULL,
			       NULL);

			int iSuccess = TstrGo(ptstr, phsleModel);

			TstrDelete(ptstr);

			if (iSuccess == 1)
			{
			}
			else
			{
			    char pc[1000];

			    PidinStackString(ppistModel, pc, sizeof(pc));

			    Chemesis3Error
				(pch3,
				 NULL,
				 "Error compiling %s\n",
				 pc);

			    return(FALSE);
			}
		    }
		    else
		    {
			char pc[1000];

			PidinStackString(ppistModel, pc, sizeof(pc));

			Chemesis3Error
			    (pch3,
			     NULL,
			     "Error recording indexes when compiling %s\n",
			     pc);

			return(FALSE);
		    }
		}
		else
		{
		    char pc[1000];

		    PidinStackString(ppistModel, pc, sizeof(pc));

		    Chemesis3Error
			(pch3,
			 NULL,
			 "memory failure when compiling %s\n",
			 pc);

		    return(FALSE);
		}
	    }
	    else
	    {
		char pc[1000];

		PidinStackString(ppistModel, pc, sizeof(pc));

		Chemesis3Error
		    (pch3,
		     NULL,
		     "no pools with ion concentrations found inside %s\n",
		     pc);

		return(FALSE);
	    }
	}
	else
	{
	    char pc[1000];

	    PidinStackString(ppistModel, pc, sizeof(pc));

	    Chemesis3Error
		(pch3,
		 NULL,
		 "unable to traverse %s\n",
		 pc);

	    return(FALSE);
	}

	//- correct the serial to indexes

	iResult = iResult && solver_serials_2_indexes(pch3);

	//- complete the links

	iResult = iResult && solver_complete_indexes(pch3);
    }
    else
    {
	iResult = FALSE;
    }

    //- return result

    return(iResult);
}


int Chemesis3NeurospacesKinetics2Chemesis3(struct Chemesis3TranslationService *pcts)
{
    //- set the service function pointer to chemesis3_setup_kinetics()

    pcts->kinetic_inspector = chemesis3_setup_kinetics;

    //- return result

    return(1);
}


