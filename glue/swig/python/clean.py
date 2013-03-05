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

            found_infos = glob.glob("%s%s%s" % (_this_directory, os.sep,g))

            for i in found_infos:

                _delete(i)
            
        else:

            if os.path.exists(g):

                if os.path.isdir(g):

                    _empty_directory(g)
                    
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

def _empty_directory(directory):

    for path, dirs, files in os.walk(directory, topdown=False):

        for d in dirs:

            _empty_directory(os.path.join(path,d))

        for f in files:

            _delete(os.path.join(path,f))

        _delete(path)

#---------------------------------------------------------------------------



remove_python_garbage()
