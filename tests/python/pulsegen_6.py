#! /usr/bin/env python
"""
This test checks the struct wrapper for the pulsegen
data struct. SWIG converts it into a low level class.
"""
import os
import sys
import pdb

from test_library import print_pulsegen
from test_library import add_package_path

add_package_path("experiment")

from neurospaces.experiment.pulsegen import PulseGen

pg = PulseGen("Test name", 50.0, 3.0, 5.0, -20.0,
              5.0, 8.0, 10.0, 0)


d = 10.0
pg.AddVariable(d)
step = 0.5
num_steps = 200

for i in range(0,200):
      
    pass




print "Value in added variable is %f" % pg.GetOutput()


print "Done!"
