#! /usr/bin/env python
"""

"""
import os
import sys
import pdb

from test_library import add_package_path

add_package_path("experiment", "neurospaces")

from test_library import print_simobj_perfectclamp
from experiment.perfectclamp import PerfectClamp

pc = PerfectClamp("Test Perfect Clamp")

print "Done!"
