#! /usr/bin/env python
"""

"""

import pdb
import os
import sys

#
# Since the current directory is part of sys.path
# uninstall.py must check for it and remove it, otherwise
# it will delete the neurospaces directory in the source
# directory.
#
_this_directory =  os.path.dirname(os.path.abspath(__file__))

try:

    sys.path.remove(_this_directory)

except ValueError, e:

    print "The current directory is not in the system path"


try:

    sys.path.remove('.')

except ValueError:

    pass

try:
    
    from neurospaces.packages import PackageManager

except ImportError:

    print "Experiment package is not installed."
    
    sys.exit(0)

package_manager = PackageManager(verbose=False)

package_manager.Uninstall('experiment')

