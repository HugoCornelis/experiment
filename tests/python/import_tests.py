#! /usr/bin/env python
"""
This test checks the struct wrapper for the pulsegen
data struct. SWIG converts it into a low level class.
"""
import os
import sys
import pdb

root_path = os.environ['HOME'] + "/neurospaces_project/model_container/source/snapshots/0"

nmc_path = root_path + "/glue/swig/python/"

sys.path.append(nmc_path)


print "Importing nmc_base"
import nmc.nmc_base


print "Importing nmc"
import nmc

print "Done!"
