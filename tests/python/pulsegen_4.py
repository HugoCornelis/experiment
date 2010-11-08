#! /usr/bin/env python
"""
This test checks the struct wrapper for the pulsegen
data struct. SWIG converts it into a low level class.
"""
import os
import sys
import pdb


root_path = os.environ['HOME'] + "/neurospaces_project/experiment/source/snapshots/0"

pulsegen_path = root_path + "/glue/swig/python/experiment/pulsegen"

sys.path.append(pulsegen_path)

from pulsegen_base import fact

var = 100
varout = fact(var)

print "var out is %d" % varout

#print_pulsegen(pg)

print "Done!"
