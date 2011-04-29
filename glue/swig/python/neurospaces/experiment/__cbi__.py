"""
@file __cbi__.py

This file provides data for a packages integration
into the CBI architecture.
"""

__author__ = "Mando Rodriguez"
__copyright__ = "Copyright 2010, The GENESIS Project"
__credits__ = ["Hugo Cornelis", "Dave Beeman"]
__license__ = "GPL"
__version__ = "0.1"
__maintainer__ = "Mando Rodriguez"
__email__ = "rodrigueza14@uthscsa.edu"
__status__ = "Development"
__url__ = "http://genesis-sim.org"
__description__ = """
The experiment module houses experimental protocols for use
in simulations in GENESIS 3. 
"""
__download_url__ = "http://repo-genesis3.cbi.utsa.edu"

def GetRevisionInfo():
# $Format: "    return \"${monotone_id}\""$
    return "bf8d4762270a230e4a784b9c6f4133ca7f8134c0"

def GetPackageName():
# $Format: "    return \"${package}\""$
    return "experiment"

def GetVersion():
# $Format: "    return \"${major}.${minor}.${micro}-${label}\""$
    return "0.0.0-alpha"

def GetDependencies():
    """!
    @brief Provides a list of other CBI dependencies needed.
    """
    dependencies = []

    return dependencies
