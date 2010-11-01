#! /usr/bin/env python
"""

"""
import os
import sys
import pdb

root_path = os.environ['HOME'] + "/neurospaces_project/experiment/source/snapshots/0"

pulsegen_path = root_path + "/glue/swig/python/experiment/pulsegen"

sys.path.append(pulsegen_path)

import pulsegen_base


experiment_path = root_path + "/glue/swig/python/experiment"

sys.path.append(experiment_path)

from pulsegen import PulseGen


print "Done!"
