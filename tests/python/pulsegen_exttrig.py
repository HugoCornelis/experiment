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
from experiment.pulsegen import EXT_TRIG

pg = PulseGen("Test name", 50.0, 3.0, 5.0, -20.0,
              5.0, 8.0, 10.0, 0)


pg.SetTriggerMode(EXT_TRIG)

dtime = 0.0
steps = 200
stepsize = 0.5
pulseout = 0.0

pg.AddVoidVariable(pulseout)

for i in range(0,200):

  print "%.1f %d" % (dtime,pg.GetOutput())
  pg.SingleStep(dtime)
  dtime += stepsize

print "Done!"
