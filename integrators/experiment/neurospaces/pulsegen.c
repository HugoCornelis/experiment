

#include <stdio.h>
#include <stdlib.h>

#include <neurospaces/pidinstack.h>
#include <neurospaces/treespacetraversal.h>

#include "chemesis3/addressing.h"
#include "chemesis3/neurospaces/chemesis3.h"
#include "chemesis3/service.h"


/********************** prototypes *********************************************/

static int pulsegen_processor(struct TreespaceTraversal *ptstr, void *pvUserdata);


/********************** end prototypes *****************************************/



static
int
pulsegen_processor(struct TreespaceTraversal *ptstr, void *pvUserdata)
{
    //- set default result : ok

    int iResult = TSTR_PROCESSOR_SUCCESS;

    //- get pointer to solver

    struct kinetics_info *pki = (struct kinetics_info *)pvUserdata;

    struct simobj_Chemesis3 *pch3 = pki->pch3;

    //- get actual symbol

    int iType = TstrGetActualType(ptstr);

    struct symtab_HSolveListElement *phsle = (struct symtab_HSolveListElement *)TstrGetActual(ptstr);


    if( instanceof_pulse_gen(iType))
    {


    }


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
