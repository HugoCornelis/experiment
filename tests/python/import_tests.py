#! /usr/bin/env python
"""
This test checks the struct wrapper for the pulsegen
data struct. SWIG converts it into a low level class.
"""
import os
import sys
import pdb

root_path = os.environ['HOME'] + "/neurospaces_project/experiment/source/snapshots/0"

nmc_path = root_path + "/glue/swig/python/"

sys.path.append(nmc_path)


print "Importing pulsegen_base"
import experiment.pulsegen_base


print "Importing pulsegen"
import experiment.pulsegen


print "Importing perfectclamp_base"
import experiment.perfectclamp_base

print "Importing perfectclamp"
import experiment.perfectclamp

print "Done!"
