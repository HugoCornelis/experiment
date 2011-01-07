"""
@file __cbi__.py

This file provides data for a packages integration
into the CBI architecture.
"""


__author__ = 'Mando Rodriguez'
__credits__ = []
__license__ = "GPL"
__version__ = "0.1"
__status__ = "Development"

def GetRevisionInfo():
# $Format: return "${monotone_id}"$
    return "test"

def GetPackageName():
# $Format: return "${package}"$
    return "test"

def GetVersion():
# $Format: return "${major}.${minor}.${micro}-${label}"$
    return "test"

def GetDependencies():
    """!
    @brief Provides a list of other CBI dependencies needed.
    """
    dependencies = ['heccer']

    return dependencies
