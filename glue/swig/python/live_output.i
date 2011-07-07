/* -*- c -*- */

/**********************************************************
python list methods from http://www.swig.org/Doc1.3/Python.html


PyObject *PyList_New(int size);
int       PyList_Size(PyObject *list);
PyObject *PyList_GetItem(PyObject *list, int i);
int       PyList_SetItem(PyObject *list, int i, PyObject *item);
int       PyList_Insert(PyObject *list, int i, PyObject *item);
int       PyList_Append(PyObject *list, PyObject *item);
PyObject *PyList_GetSlice(PyObject *list, int i, int j);
int       PyList_SetSlice(PyObject *list, int i, int , PyObject *list2);
int       PyList_Sort(PyObject *list);
int       PyList_Reverse(PyObject *list);
PyObject *PyList_AsTuple(PyObject *list);
int       PyList_Check(PyObject *);

Link to the PyString API functions: http://docs.python.org/c-api/string.html
***********************************************************/


%{
#include <float.h>
#include <Python.h>

#define ALLOCATE_VARIABLES 10000

struct LiveOutput
{
    /// number of variables

    int iVariablesAllocated;

    int iVariablesActive;

    /// all variables

    double **ppdVariables;

    /// base value: only values different from this are recorded

    double dBase;

    /// steps mode

    int iSteps;

    /// output format

    char *pcFormat;

    /// output resolution: modulo factor

    int iResolution;

    /// output resolution: current step

    int iResolutionStep;

/*     /// simulation time */

/*     double dSimulationTime; */

    /*
     * This data member will be a list of lists
     */
    PyObject *ppoData;

};


%}


/***************************************************
* Imports and Typemaps
***************************************************/

%import "cpointer.i"

%pointer_functions(double,pdouble);

%import "typemaps.i"


%inline %{

//------------------------------------------ Prototypes -------------------------------

int
LiveOutputAddVariable
(struct LiveOutput * plo, char *pcName, double *pdVariable);

int LiveOutputInitiate(struct LiveOutput * plo);

struct LiveOutput * LiveOutputNew();

int LiveOutputSetSteps(struct LiveOutput * plo, int iSteps);

int LiveOutputTimedStep(struct LiveOutput * plo, double dTime);

PyObject * LiveOutputData(struct LiveOutput * plo);

//------------------------------------------ End Prototypes ---------------------------



//------------------------------------------------------------------------------
/*
 * Returns a tuple of coodinates in the form (x, y z)
 * Attempts cleanup of objects in the event of an error.
 */
static PyObject * CoordinateTuple(double dX, double dY, double dZ)
{

  PyObject * ppoTuple = PyTuple_New(3);
  
  if( !ppoTuple )
  {
    return NULL;
  }

  PyObject * ppoX = NULL;
  PyObject * ppoY = NULL;
  PyObject * ppoZ = NULL;

  ppoX = PyFloat_FromDouble(dX);

  if( !PyFloat_Check(ppoX) )
  {
    free(ppoX);
    return NULL;
  }

  ppoY = PyFloat_FromDouble(dY);

  if( !PyFloat_Check(ppoY) )
  {
    free(ppoX);
    free(ppoY);
    return NULL;
  }

  ppoZ = PyFloat_FromDouble(dZ);
  
  if( !PyFloat_Check(ppoZ) )
  {
    free(ppoX);
    free(ppoY);
    free(ppoZ);
    return NULL;
  }

  ppoTuple = PyTuple_New(3);

  if( !ppoTuple )
  {
    free(ppoX);
    free(ppoY);
    free(ppoZ);
    return NULL;
  }

  PyTuple_SetItem(ppoTuple, 0, ppoX);
  PyTuple_SetItem(ppoTuple, 1, ppoY);
  PyTuple_SetItem(ppoTuple, 2, ppoZ);

  if( !PyTuple_Check(ppoTuple) )
  {
    free(ppoX);
    free(ppoY);
    free(ppoZ);
    return NULL;
  }

  return ppoTuple;

}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/*
 * 
 */
int
LiveOutputAddVariable
(struct LiveOutput * plo, char *pcName, double *pdVariable)
{
    //- set default result : ok

    int iResult = 1;

    if (plo->iVariablesActive >= ALLOCATE_VARIABLES)
    {
	return(0);
    }

    //- set next variable

    plo->ppdVariables[plo->iVariablesActive] = pdVariable;

    plo->iVariablesActive++;

    //- return result

    return(iResult);
}
//------------------------------------------------------------------------------





//------------------------------------------------------------------------------
/*
 * 
 */
int LiveOutputInitiate(struct LiveOutput * plo)
{
    //- set default result : ok

    int iResult = 1;

    //- Is currently a do nothing since there's
    //- no file to check for. 

    //- return result
    //- Create an empty python list

    if( !plo )
    {
      
      return 0;

    }


    if( !plo->ppoData )
    {
      plo->ppoData = PyList_New(0);
      
      if( !plo->ppoData || !PyList_Check(plo->ppoData) )
      {
	PyErr_SetString(PyExc_MemoryError,"can't allocate list output data list");
      
	free(plo->ppdVariables);
	free(plo);
      
	iResult = 0;
      }
      
    }

    return(iResult);
}
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
/*
 * 
 */
struct LiveOutput * LiveOutputNew()
{
    //- set default result : failure

    struct LiveOutput * ploResult = NULL;

    //- allocate output generator

    ploResult = (struct LiveOutput *)calloc(1, sizeof(struct LiveOutput));

    if (!ploResult)
    {
	return(NULL);
    }

    //- allocate output variables

    ploResult->ppdVariables = (double **)calloc(ALLOCATE_VARIABLES, sizeof(double *));

    ploResult->iVariablesAllocated = ALLOCATE_VARIABLES;

    if (!ploResult->ppdVariables)
    {
	free(ploResult);

	return(NULL);
    }

    //- initialize base value: DBL_MAX

    ploResult->dBase = DBL_MAX;

    //- initialize: non-steps mode

    ploResult->iSteps = 0;


    //- Create an empty python list

    ploResult->ppoData = PyList_New(0);


    if( !ploResult->ppoData || !PyList_Check(ploResult->ppoData) )
    {
      PyErr_SetString(PyExc_MemoryError,"can't allocate list output data list");
      
      free(ploResult->ppdVariables);
      free(ploResult);

      return NULL;
    }

    //- return result

    return(ploResult);
}
//------------------------------------------------------------------------------




//------------------------------------------------------------------------------
/*
 * 
 */
int LiveOutputSetSteps(struct LiveOutput * plo, int iSteps)
{
    //- set default result : ok

    int iResult = 1;

    //- set format

    plo->iSteps = iSteps;

    //- return result

    return(iResult);
}

//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
/*
 * 
 */
int LiveOutputTimedStep(struct LiveOutput * plo, double dTime)
{
    int i;
    int iResult = 1;
    int iOutput = 1;
    double dTmp;
    char pcVariables[1000] = "";
    PyObject *ppoList = NULL;
    PyObject *ppoVariable = NULL;
    PyObject *ppoTimeStamp = NULL;
    PyObject *ppoOutputTuple = NULL;

    //- if we are not at the correct resolution time step

    if (plo->iResolution > 0)
    {
	plo->iResolutionStep--;

	if (plo->iResolutionStep > 0)
	{
	    //- nothing to output, return success

	    return(1);
	}

	plo->iResolutionStep = plo->iResolution;
    }

    //- Create a python tuple for our data

    ppoOutputTuple = PyTuple_New(2);

    if( !ppoOutputTuple || !PyTuple_Check(ppoOutputTuple) )
    {

      PyErr_SetString(PyExc_MemoryError,"Can't allocate tuple for timed step output");

    }    

    //- Create an empty python list

    ppoList = PyList_New(0);


    if( !ppoList || !PyList_Check(ppoList) )
    {
      PyErr_SetString(PyExc_MemoryError,"Can't allocate list for timed step");

      //- Should never get here, python will bail out before then

      return 0;
    }

    ppoTimeStamp = PyFloat_FromDouble(dTime);

    if( !PyFloat_Check(ppoTimeStamp) )
    {

      PyErr_SetString(PyExc_MemoryError,"Can't allocate float for timestamp");

    }

    //- copy all the variables to our collective list

    for (i = 0 ; i < plo->iVariablesActive ; i++)
    {
	//- if the variable differs from the base value

	if (*plo->ppdVariables[i] != plo->dBase)
	{

	  dTmp = *plo->ppdVariables[i];

	  ppoVariable = PyFloat_FromDouble(dTmp);

	  if( !PyFloat_Check(ppoVariable) )
	  {
	    
	    PyErr_SetString(PyExc_MemoryError,"Can't read variable from address");

	  }

	  PyList_Append(ppoList, ppoVariable);

	  iOutput = 1;

	}
    }

    //- if there was any output

    if (iOutput)
    {

      if( !PyList_Check(ppoList) )
      {

	PyErr_SetString(PyExc_MemoryError,"Collected variable data list is invalid");

	//- Should never get here, python will bail out before then

	return 0;
      }

      //- copy this python list to the data tuple list
      
      //- First set the items into the tuple
      PyTuple_SetItem(ppoOutputTuple, 0, ppoTimeStamp);
      PyTuple_SetItem(ppoOutputTuple, 1, ppoList);


      PyList_Append(plo->ppoData, ppoOutputTuple);

    }

    //- return result

    return(iResult);
}


//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
/*
 * 
 */
PyObject * LiveOutputData(struct LiveOutput * plo)
{

  if( !plo )
  {

    return NULL;

  }

  return plo->ppoData;

}

//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
/*
 * 
 */
//------------------------------------------------------------------------------

%}

