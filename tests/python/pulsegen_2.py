#! /usr/bin/env python
"""
This test checks the struct wrapper for the pulsegen
data struct. SWIG converts it into a low level class.
"""
import os
import sys
import pdb


sys.path.append('./glue/swig/python/experiment')

from test_library import print_pulsegen
from pulsegen import PulseGen

pg = PulseGen("Test name", 50.0, 3.0, 5.0, -20.0,
              5.0, 8.0, 10.0, 0)

print_pulsegen(pg)

print "Done!"
