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

except ImportError:
    sys.exit("Could not import compiled SWIG pulsegen_base library: %s")


class PulseGen:

    def __init__(self,level1=0, width1=0, delay1=0,
                 level2=0, width2=0, delay2=0,
                 base_level=0, trigger_mode=0):
        
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

    
