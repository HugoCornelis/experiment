
import os
import pdb
import sys

try:
    
import output_base

except ImportError, e:

    sys.exit("Could not import compiled SWIG output_base library: %s", e)

class Output:
    """!
    @class Output
    """

    def __init__(self, filename):
        """!

        """
        self._og = output_base.OutputGeneratorNew(filename)

    
    def Advance(self, time):
        """!

        """
        result = output_base.OutputGeneratorTimedStep(self._og, time)

        return result

    def Step(self, time):
        """!

        """
        return self.Advance(time)
    

    def Compile(self):
        """!

        """
        result = output_base.OutputGeneratorInitiate(self._og)

        return result
    

    def AddOutput(self, name, address):
        """!

        """
        output_base.OutputGeneratorAddVariable(self._og, name, address)

    def Finish(self):
        """!

        """
        output_base.OutputGeneratorFinish(self._og)



class OutputGenerator(output_base.OutputGenerator):
    """!
    @class Output
    """

    def __init__(self, filename):
        """!

        """

        output_base.OutputGenerator.__init__(self)
        
        self._og = output_base.OutputGeneratorNew(filename)

    
    def Advance(self, time):
        """!

        """
        result = output_base.OutputGeneratorTimedStep(self._og, time)

        return result

    def Step(self, time):
        """!

        """
        return self.Advance(time)
    

    def Compile(self):
        """!

        """
        result = output_base.OutputGeneratorInitiate(self._og)

        return result
    

    def AddOutput(self, name, address):
        """!

        """
        output_base.OutputGeneratorAddVariable(self._og, name, address)

    def Finish(self):
        """!

        """
        output_base.OutputGeneratorFinish(self._og)
