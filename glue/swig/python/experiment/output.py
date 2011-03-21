
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
    @class Output
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
