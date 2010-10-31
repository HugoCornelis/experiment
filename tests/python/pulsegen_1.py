#! /usr/bin/env python
"""
This test checks the struct wrapper for the pulsegen
data struct. SWIG converts it into a low level class.
"""
import os
import sys
import pdb


sys.path.append('./glue/swig/python/experiment/pulsegen')

from test_library import print_simobj_pulsegen
from pulsegen_base import simobj_PulseGen

spg = simobj_PulseGen()

spg.pcName = "Test name"
spg.dLevel1 = 50.0
spg.dWidth1 = 3.0
spg.dDelay1 = 5.0
spg.dLevel2 = -20.0
spg.dWidth2 = 5.0
spg.dDelay2 = 8.0
spg.dBaseLevel = 10.0
spg.iTriggerMode = 0

print_simobj_pulsegen(spg)

print "Done!"
