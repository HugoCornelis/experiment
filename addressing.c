

#include <string.h>


#include "experiment/addressing.h"




/// 
/// \arg pch3 a chemesis3 solver.
/// \arg iSerial identification number.
/// \arg pcType name of requested variable.
/// 
/// \return double *
/// 
///	pointer to the requested field, NULL for failure.
/// 
/// \brief Find the simulation sequence of a given variable.
///
/// \details 
/// 
///	In all cases, as of the moment of writing, the pointer is a
///	pointer to a double.
/// 

double *
PulseGenAddressVariable
(struct simobj_PulseGen *pch3, int iSerial, char *pcType)
#ifdef PULSEGEN_SOURCE_NEUROSPACES
{
    //- set default result : not found

    double *pdResult = NULL;

    if (pch3->iErrorCount)
    {
	return(NULL);
    }

    //- convert serial to chemesis3 space

    iSerial = ADDRESSING_NEUROSPACES_2_PULSEGEN(iSerial);

    //- if this is the serial of the model itself

    if (iSerial == pch3->iSerialStart)
    {
	//- address as an aggregator

	pdResult = PulseGenAddressAggregator(pch3, iSerial, pcType);

	return(pdResult);
    }

    //- if serial not within range

    if (iSerial < pch3->iSerialStart
	|| iSerial > pch3->iSerialEnd)
    {
	//- return failure

	return(NULL);
    }

    //- convert the serial to an intermediary index

/*     int iIntermediary = PulseGenAddressSerial2Intermediary(pch3, iSerial, pcType); */

    int iIntermediary = iSerial;

    if (iIntermediary != -1)
    {
	//- for reaction addressables

	if (strcasecmp(pcType, "forward_rate") == 0
	    || strcasecmp(pcType, "backward_rate") == 0)
	{
	    pdResult = PulseGenAddressReactionVariable(pch3, iIntermediary, pcType);
	}
	else
	{
	    pdResult = PulseGenAddressPoolVariable(pch3, iIntermediary, pcType);
	}
    }
    else
    {
	/// \note this cannot be, internal error


	//- return error

	return(NULL);
    }

    //- return result

    return(pdResult);
}
#else
{
    return(NULL);
}
#endif



