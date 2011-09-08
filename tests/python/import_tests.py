#! /usr/bin/env python
"""
This test checks the struct wrapper for the pulsegen
data struct. SWIG converts it into a low level class.
"""
import os
import sys
import pdb


from test_library import add_package_path

add_package_path("experiment")


print "Importing pulsegen_base"
import experiment.pulsegen_base


print "Importing pulsegen"
import experiment.pulsegen


print "Importing perfectclamp_base"
import experiment.perfectclamp_base

print "Importing perfectclamp"
import experiment.perfectclamp

print "Done!"
