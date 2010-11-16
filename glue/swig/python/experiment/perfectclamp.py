"""

The perfect clamp module provides an abstraction
that encapsulates all of the complex operations of
the perfectclamp C code.
"""
import os
import pdb
import sys


from perfectclamp_base import PerfectClampAddVariable
from perfectclamp_base import PerfectClampFinish
from perfectclamp_base import PerfectClampInitiate
from perfectclamp_base import PerfectClampNew
from perfectclamp_base import PerfectClampSetFields
from perfectclamp_base import PerfectClampSingleStep

from perfectclamp_base import new_pdouble
from perfectclamp_base import copy_pdouble
from perfectclamp_base import delete_pdouble
from perfectclamp_base import pdouble_assign
from perfectclamp_base import pdouble_value

__author__ = 'Mando Rodriguez'
__credits__ = []
__license__ = "GPL"
__version__ = "0.1"
__status__ = "Development"

class PerfectClamp:

    def __init__(self,name):

        self.pc = PerfectClampNew(name)

        # this will be the solved variable
        self.voltage = sys.maxint

        # exception?

    def SetCommandVoltage(self,command):

        self.pc.dCommand = command

    def GetCommandVoltage(self):

        return self.pc.dCommand

    def GetVoltage(self):

        return pdouble_value(self.pc.pdVoltage)
    
    def SetFields(self,command,filename):

        result = PerfectClampSetFields(self.pc,
                                       command,
                                       filename)
        # extra error checking?
        return result

    def SingleStep(self,time):

        # error cheking?
        result = PerfectClampSingleStep(self.pc,time)

        return result


    
    def Finish(self):

        result = PerfectClampFinish(self.pc)

        return result


    def Initiate(self):

        result = PerfectClampInitiate(self.pc)

        return result
            

    def AddVariable(self,voltage):
        """
        Add a solved variable.
        """
        
        result = PerfectClampAddVariable(self.pc,voltage)

        return result
