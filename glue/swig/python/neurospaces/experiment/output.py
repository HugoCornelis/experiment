
import os
import pdb
import sys

try:
    
    import output_base

except ImportError, e:

    sys.exit("Could not import compiled SWIG output_base library: %s", e)


#---------------------------------------------------------------------------

class Output:
    """!
    @class Output A class for the output generator.
    """

#---------------------------------------------------------------------------

    def __init__(self, filename):
        """!

        """
        self.filename = filename

        self.initiated = False
        
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

    def Advance(self, time):
        """!

        """
        result = output_base.OutputGeneratorTimedStep(self._og, time)

        return result


#---------------------------------------------------------------------------

    def SetFormat(self, fmt):
        """
        @brief Sets the string format options
        """

        output_base.OutputGeneratorSetFormat(self._og, fmt)

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

    def Step(self, time):
        """!

        """
        return self.Advance(time)

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


#---------------------------------------------------------------------------
