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

from experiment.perfectclamp import PerfectClamp


pc = PerfectClamp("Test clamp")


dtime = 0.0
steps = 200
stepsize = 0.5
voltage_out = 0.0
command_voltage = -0.06

pc.SetCommandVoltage(command_voltage)

pc.AddVariable(voltage_out)

for i in range(0,200):

  print "%.1f %d" % (dtime,pc.GetVoltage())
  pc.Step(dtime)
  dtime += stepsize

print "Done!"
