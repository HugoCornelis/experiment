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


