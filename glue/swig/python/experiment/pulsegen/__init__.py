"""!
This is the module used to create a pulsegen python object
from the swig bindings.
"""
import os
import sys
import pdb

__author__ = 'Mando Rodriguez'
__credits__ = []
__license__ = "GPL"
__version__ = "0.1"
__status__ = "Development"

try:

    import pulsegen_base

except ImportError as error:
    sys.exit("Could not import compiled SWIG pulsegen_base library: %s" % error)


class PulseGen:

    def __init__(self):
        print "Initializing a pulsegen"
        pass


    def SetLevel1(self,level1):
        pass

    def SetWidth1(self,width1):
        pass

    def SetDelay1(self,delay1):
        pass

    def SetLevel2(self,level2):
        pass

    def SetWidth2(self,width2):
        pass

    def SetDelay2(self,delay2):
        pass

    def SetBaseLevel(self,base_level):
        pass

    def SetTriggerMode(self,trigger_mode):
        pass

    def SingleStep(self):
        pass

    def Reset(self):
        pass

    def Finish(self):
        pass

    def AddInput(self,input):
        pass

    def AddVariable(self,variable):
        pass

    
