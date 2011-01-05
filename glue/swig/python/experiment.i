%module experiment_base



%{
#include "experiment/config.h"
#include "experiment/output.h"
#include "experiment/perfectclamp.h"

%}

%inline %{


%}

%include "experiment/config.h"
%include "experiment/output.h"
%include "experiment/perfectclamp.h"

 // info on the carray library here http://www.swig.org/Doc1.3/Library.html#Library_carrays
%include "carrays.i"
%array_functions(double, DoubleArray)
%array_functions(int, IntArray)

