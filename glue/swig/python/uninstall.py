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

def remove_egg(module_name):
    """
    finds all easy path files, collects all paths to eggs that
    are installed and removes all of them.
    """
    from commands import getoutput
    import glob
    import re
    
    installs = []

    for path in sys.path:

        # Check for an easy-install.pth file and a module with the name

        easy_install_file = os.path.join(path, 'easy-install.pth')
                                         
        found_eggs = glob.glob("%s%s%s*.egg" % (path, os.sep, module_name))
        
        found_infos = glob.glob("%s%s%s*.egg-info" % (path, os.sep, module_name))

        egg_data = []
        egg_data.extend(found_eggs)
        egg_data.extend(found_infos)
        
        if os.path.isfile(easy_install_file) and len(found_eggs) > 0:

            installs.append(dict(pth_file=easy_install_file,
                                 eggs=egg_data))

    if len(installs) == 0:

        print "No python eggs found."

        return

    else:
        
        print "Found %d python eggs installed" % len(installs)

        for inst in installs:

            pth_file = inst['pth_file']
            eggs = inst['eggs']

            f = open(pth_file, 'r')

            pth_data = f.read()

            tmp_data = pth_data.split('\n')

            pth_lines = []
            
            for line in tmp_data:

                if not re.search("\S*%s\-\S*\.egg$" % module_name, line):

                    pth_lines.append(line)

            pth_data = '\n'.join(pth_lines)
            
            f.close()
            
            # Remove the egg and easy-install.pth file

            remove_these_files  = []

            remove_these_files.extend(eggs)
            remove_these_files.append(pth_file)

            print "Removing the following files: \n%s" % '\n'.join(remove_these_files)

            if os.access(remove_these_files[0], os.W_OK):

                cmdout = getoutput("rm -rf %s" % ' '.join(remove_these_files))

            else:

                cmdout = getoutput("sudo rm -rf %s" % ' '.join(remove_these_files))

            print cmdout

            print "Writing a new easy-install.pth file: %s" % pth_file
            
            f = open(pth_file, 'w')

            f.write(pth_data)

            f.close()

            if not os.path.isfile(pth_file):

                raise Exception("An error occured, the easy-path.pth file wasn't written")
        

remove_egg('experiment')

try:
    
    from neurospaces.packages import PackageManager

except ImportError:

    print "Experiment package is not installed."
    
    sys.exit(0)

package_manager = PackageManager(verbose=False)

try:
    
    package_manager.uninstall('experiment')

except Exception, e:

    print "Can't uninstall experiment: %s" % e

