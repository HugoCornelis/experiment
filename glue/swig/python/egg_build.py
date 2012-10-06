
import subprocess
import sys

setuptools_present = True

try:
    
    import setuptools
    
except ImportError:

    setuptools_present = False


if setuptools_present:

    print "setuptools is present, we can run bdist_egg:"
    
    command = [sys.executable, 'setup.py', 'bdist_egg']

    subprocess.call(command)

    print "Done running egg build"

else:

    print "setuptools not present, no need to call bdist_egg"
