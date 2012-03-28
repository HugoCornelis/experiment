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

pg = PulseGen("Test name", 50.0, 3.0, 5.0, -20.0,
              5.0, 8.0, 10.0, 0)


pg.SetTriggerMode(FREE_RUN)

dtime = 0.0
steps = 200
stepsize = 0.5
pulseout = 0.0

pg.AddVariable(pulseout)

f = open("/tmp/output", 'w')

for i in range(0,200):

  f.write("%.1f %d\n" % (dtime,pg.GetOutput()))
  pg.SingleStep(dtime)
  dtime += stepsize

f.close()
print "Done!"
