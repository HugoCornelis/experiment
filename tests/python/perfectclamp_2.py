#! /usr/bin/env python
"""

"""
import os
import sys
import pdb

root_path = os.environ['HOME'] + "/neurospaces_project/experiment/source/snapshots/0"

experiment_path = root_path + "/glue/swig/python"

sys.path.append(experiment_path)

from test_library import print_simobj_perfectclamp
from neurospaces.experiment.perfectclamp import PerfectClamp

pc = PerfectClamp("Test Perfect Clamp")

print "Done!"
