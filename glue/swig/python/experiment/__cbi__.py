"""
@file __cbi__.py

This file provides data for a packages integration
into the CBI architecture.
"""

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
    dependencies = []

    return dependencies
