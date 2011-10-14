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

from commands import getoutput

#---------------------------------------------------------------------------

def remove_python_garbage():
    """
    Deletes all of the python generated files.
    """
    import glob
    import re

    garbage = ['build','*.egg-info','dist']

    for g in garbage:

        delete_me = ''
        
        if re.search("\*", g):
            # If this is a pattern then we use glob.

            found_infos = glob.glob("%s%s*.egg-info" % (_this_directory, os.sep))

            for i in found_infos:

                _delete(i)
            
        else:

            if os.path.exists(g):
                
                _delete(g)

    print "Done deleting python build garbage"
    
#---------------------------------------------------------------------------

def _delete(path):

    print "Removing: %s" % path

    if os.access(path, os.W_OK):

        cmdout = getoutput("rm -rf %s" % path)

    else:

        cmdout = getoutput("sudo rm -rf %s" % path)

    print cmdout

#---------------------------------------------------------------------------



remove_python_garbage()
