
import os
import pdb
import sys

try:
    
    import output_base

except ImportError, e:

    sys.exit("Could not import compiled SWIG output_base library: %s" % e)


#***************************************************************************

class Output:
    """!
    @class Output A class for the output generator.
    """

#---------------------------------------------------------------------------

    def __init__(self, filename='/tmp/output', verbose=False):
        """!

        """
        self.filename = filename

        self.initiated = False

        self.verbose = verbose
        
        self._og = output_base.OutputGeneratorNew(filename)

        if self._og is None:

            raise Exception("Error: Can't create Output Generator for file '%s'\n" % self.filename)


        self.Initialize()
        
#---------------------------------------------------------------------------

    def Initialize(self):

        if not self.initiated:
            
            result = output_base.OutputGeneratorInitiate(self._og)

            if result == 0:

                raise Exception("Can't create file '%s' for Output Generator\n" % self.filename)

            self.initiated = True

#---------------------------------------------------------------------------

    def Step(self, time):
        """!

        """
        result = output_base.OutputGeneratorTimedStep(self._og, time)

        return result
    


#---------------------------------------------------------------------------

    def SetResolution(self, resolution):
        """
        @brief Sets the output resolution
        """

        self._og.iResolution = resolution

#---------------------------------------------------------------------------

    def SetSteps(self, steps):
        """
        @brief Turns on/off steps mode.

        1 to turn on steps mode, 0 to turn it off.
        """
        output_base.OutputGeneratorSetSteps(self._og, steps)
        



#---------------------------------------------------------------------------

    def SetFormat(self, fmt):
        """
        @brief Sets the string format options
        """
        
        output_base.OutputGeneratorSetFormat(self._og, fmt)
        
#---------------------------------------------------------------------------    

    def Compile(self):
        """!

        """
        self.Initialize()
    
#---------------------------------------------------------------------------

    def AddOutput(self, name, address):
        """!

        """
        output_base.OutputGeneratorAddVariable(self._og, name, address)

#---------------------------------------------------------------------------

    def Finish(self):
        """!

        """
        output_base.OutputGeneratorFinish(self._og)


#---------------------------------------------------------------------------

# An alias
OutputGenerator = Output


#***************************************************************************

class LiveOutput:
    """
    """
    
#---------------------------------------------------------------------------

    def __init__(self, verbose=False):
        """!

        """

        self.initiated = False

        self.verbose = verbose
        
        self._lo = output_base.LiveOutputNew()

        self.column_order = False
        
        self.output_data = []

        self.Initialize()
        
#---------------------------------------------------------------------------

    def Initialize(self):

        if not self.initiated:
            
            result = output_base.LiveOutputInitiate(self._lo)

            if result == 0:

                raise Exception("Can't create live output")

            self.initiated = True

#---------------------------------------------------------------------------

    def Step(self, time):
        """!

        """
        output = output_base.LiveOutputTimedStepVolatile(self._lo, time)

        if output is None:

            if self.verbose:
                
                print "No data output on timestep %f" % time

        else:

            if self.column_order:

                self._ColumnStep(output)

            else:
            
                self.output_data.append(output)

#---------------------------------------------------------------------------

    def _ColumnStep(self, output_list):


        if len(self.output_data) > 0:
            # If there's nothing in the data struct yet
            # then we need to create a set of arrays, one for
            # each data entry to become a column

            self.output_data = [[] for i in xrange(len(output_list))]
            
        for indx, o in enumerate(output_list):

            self.output_data[indx].append(o)
                
        
#---------------------------------------------------------------------------

    def SetOrder(self, order_type):

        if order_type == 'column':

            self.column_order = True

        elif order_type == 'row':

            self.column_order = False

        else:

            raise Exception("Invalid option, only 'row' or 'column' accepted")
        
#---------------------------------------------------------------------------

    def SetResolution(self, resolution):
        """
        @brief Sets the output resolution
        """

        output_base.SetResolution(self._lo, resolution)

#---------------------------------------------------------------------------

    def GetResolution(self):

        return self._lo.iResolution

#---------------------------------------------------------------------------

    def SetSteps(self, steps):
        """
        @brief Turns on/off steps mode.

        1 to turn on steps mode, 0 to turn it off.
        """
        output_base.LiveOutputSetSteps(self._lo, steps)

#---------------------------------------------------------------------------    

    def Compile(self):
        """!

        """
        self.Initialize()
    
#---------------------------------------------------------------------------

    def AddOutput(self, name, address):
        """!

        """
        output_base.LiveOutputAddVariable(self._lo, name, address)

#---------------------------------------------------------------------------

    def Finish(self):
        """!
        @brief empty method

        This is just left in for consistency
        """
        self.output_base = []

#---------------------------------------------------------------------------

    def GetData(self):

        return self.output_data

#***************************************************************************

class LineOutput(LiveOutput):
    """
    @brief an output object that produces only one line at a time

    An inherited class from the LiveOutput class. This does not
    keep a persistent data structure with output values, it only
    outputs one line of output in the form of an array with the
    first value as the timestamp and following values the output
    variables. 
    """
    
    def __init__(self):

        LiveOutput.__init__(self)

#---------------------------------------------------------------------------


    def Step(self, time):
        """!

        """
        output_data = output_base.LiveOutputTimedStepVolatile(self._lo, time)

        return output_data

#---------------------------------------------------------------------------


#***************************************************************************

class StdOut(LiveOutput):
    """
    @brief an output object that produces only one line at a time

    An inherited class from the LiveOutput class. This does not
    keep a persistent data structure with output values, it only
    outputs one line of output in the form of an array with the
    first value as the timestamp and following values the output
    variables. 
    """
    
    def __init__(self):

        LiveOutput.__init__(self)

#---------------------------------------------------------------------------


    def Step(self, time):
        """!

        """
        output_data = output_base.LiveOutputTimedStepVolatile(self._lo, time)

        try:
            
            print "time %s: %s" % (output_data[0], ','.join(map(str, output_data[1:])))

        except IndexError:

            print "time %s: No outputs" % (output_data[0])

        return 

#---------------------------------------------------------------------------


