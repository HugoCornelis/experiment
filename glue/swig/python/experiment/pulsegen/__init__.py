import os
import sys
import pdb

sys.path.append( os.path.join( os.getcwd(), '.' ) )

try:

    import pulsegen_base

except ImportError:
    sys.exit("Could not import compiled SWIG pulsegen_base library")


class PulseGen:

    def __init__(self):
        print "Initializing a pulsegen"
        pass


