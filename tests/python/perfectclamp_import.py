#! /usr/bin/env python
"""

"""
import os
import sys
import pdb

from test_library import add_package_path

add_package_path("experiment")

import experiment.perfectclamp_base

from experiment.perfectclamp import PerfectClamp


print "Done!"
