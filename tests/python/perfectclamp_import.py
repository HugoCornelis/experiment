#! /usr/bin/env python
"""

"""
import os
import sys
import pdb

root_path = os.environ['HOME'] + "/neurospaces_project/experiment/source/snapshots/0"

experiment_path = root_path + "/glue/swig/python/experiment"

sys.path.append(experiment_path)

import perfectclamp_base

from perfectclamp import PerfectClamp


print "Done!"
