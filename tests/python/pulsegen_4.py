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

from experiment.pulsegen_base import fact

var = 100
varout = fact(var)

print "var out is %d" % varout

#print_pulsegen(pg)

print "Done!"
