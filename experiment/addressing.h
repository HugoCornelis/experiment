#ifndef PULSEGEN_ADDRESSING_H
#define PULSEGEN_ADDRESSING_H



//- copy pasted this define, so hope this value holds for pulsegen

#define NEUROSPACES_2_PULSEGEN_MAX_FUNCTIONS 2

#define ADDRESSING_NEUROSPACES_2_PULSEGEN(iNeuro) ((iNeuro) << NEUROSPACES_2_PULSEGEN_MAX_FUNCTIONS)

#define ADDRESSING_PULSEGEN_2_NEUROSPACES(iPulseGen) ((iPulseGen) >> NEUROSPACES_2_PULSEGEN_MAX_FUNCTIONS

double *
PulseGenAddressVariable
(struct simobj_PulseGen *ppg, int iSerial, char *pcType);

#endif
