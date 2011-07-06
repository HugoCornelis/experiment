%module output_base


/***************************************************
* Start C code block
***************************************************/
%{
#include "experiment/output.h"

%}


/***************************************************
* Imports 
***************************************************/
%include "live_output.i"

%include "experiment/output.h"


