#! /usr/bin/env python
"""
This test checks the struct wrapper for the pulsegen
data struct. SWIG converts it into a low level class.
"""
import os
import sys
import pdb

from test_library import add_package_path
from test_library import print_pulsegen

add_package_path("experiment")

from experiment.pulsegen import PulseGen
from experiment.pulsegen import FREE_RUN


pg = PulseGen("Test name", 50.0, 3.0, 5.0, -20.0,
              5.0, 8.0, 10.0, 0)


pg.SetTriggerMode(FREE_RUN)

dtime = 0.0
steps = 12
stepsize = 0.5
pulseout = 0.0

pg.AddVariable(pulseout)

for i in range(0,steps):

  pg.SingleStep(dtime)
  dtime += stepsize

print "Output value before reset %.1f" % pg.GetOutput()

pg.Reset()

print "Output value after reset %.1f" % pg.GetOutput()

print "Done!"
