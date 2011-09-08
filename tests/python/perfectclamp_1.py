#! /usr/bin/env python
"""
This test checks the struct wrapper for the perfectclamp
data struct. SWIG converts it into a low level class.
"""
import os
import sys
import pdb

from test_library import add_package_path

add_package_path("experiment")

from test_library import print_simobj_perfectclamp
from experiment.perfectclamp_base import simobj_PerfectClamp

spc = simobj_PerfectClamp()

spc.pcName = "pc object"
spc.dCommand = 0.1
spc.iClampsActive = 1
spc.pcFilename = "My output file"

print_simobj_perfectclamp(spc)

print "Done!"
