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

from test_library import print_pulsegen
from experiment.pulsegen import PulseGen
from experiment.pulsegen import FREE_RUN

pg = PulseGen("Test name")

pg.SetFields(level1=50.0, width1=3.0, delay1=5.0,
             level2=-20.0, width2=5.0, delay2=8.0,
             base_level=10.0)
  
pg.SetTriggerMode(FREE_RUN)

dtime = 0.0
steps = 200
stepsize = 0.5
pulseout = 0.0

pg.AddVariable(pulseout)

for i in range(0,200):

  print "%.1f %d" % (dtime,pg.GetOutput())
  pg.SingleStep(dtime)
  dtime += stepsize

print "Done!"
