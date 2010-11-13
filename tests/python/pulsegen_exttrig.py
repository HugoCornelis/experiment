#! /usr/bin/env python
"""
This test checks the struct wrapper for the pulsegen
data struct. SWIG converts it into a low level class.
"""
import os
import sys
import pdb


root_path = os.environ['HOME'] + "/neurospaces_project/experiment/source/snapshots/0"

pulsegen_path = root_path + "/glue/swig/python/experiment/"

sys.path.append(pulsegen_path)

from test_library import print_pulsegen
from pulsegen import PulseGen
from pulsegen import EXT_TRIG

pg = PulseGen("Test name", 50.0, 3.0, 5.0, -20.0,
              5.0, 8.0, 10.0, 0)


pg.SetTriggerMode(EXT_TRIG)

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
