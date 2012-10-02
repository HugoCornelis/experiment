import imp
import os
import pdb
import re
import sys
from commands import getoutput

from distutils import log
from distutils.command.bdist_rpm import bdist_rpm
from distutils.command.install_data import install_data

# This will work for both setuptools and distutils
# main difference is that if distutils is imported then
# the bdist_egg option is not available.
try:
    
    from setuptools import setup, find_packages, Extension

except ImportError:

    from distutils.core import setup
    from distutils.core import Extension
    
    def find_packages():

        return ['experiment']


#from setuptools import setup, find_packages

# import the cbi module. We use this since the check
# for the compiled swig nmc_base gives an error
# if we import from nmc.__cbi__
#cbi = imp.load_source('__cbi__', os.path.join('neurospaces', 'experiment', '__cbi__.py'))

import experiment.__cbi__ as cbi
from experiment.__cbi__ import PackageInfo

_package_info = PackageInfo()



#-------------------------------------------------------------------------------

#
# This is borrowed from django's setup tools
# taken from here http://code.djangoproject.com/browser/django/trunk/setup.py
#
class osx_install_data(install_data):
    # On MacOS, the platform-specific lib dir is /System/Library/Framework/Python/.../
    # which is wrong. Python 2.5 supplied with MacOS 10.5 has an Apple-specific fix
    # for this in distutils.command.install_data#306. It fixes install_lib but not
    # install_data, which is why we roll our own install_data class.
	
    def finalize_options(self):
        # By the time finalize_options is called, install.install_lib is set to the
        # fixed directory, so we set the installdir to install_lib. The
        # install_data class uses ('install_data', 'install_dir') instead.
        self.set_undefined_options('install', ('install_lib', 'install_dir'))
        install_data.finalize_options(self)


#-------------------------------------------------------------------------------

"""
Function for reading in a file and outputting it as a string. 
"""
def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()

#-------------------------------------------------------------------------------

# borrowed from Pysco's setup.py

class ProcessorAutodetectError(Exception):
     pass
def autodetect():
     platform = sys.platform.lower()
     if platform.startswith('win'):   # assume an Intel Windows
         return 'i386'
     # assume we have 'uname'
     mach = os.popen('uname -m', 'r').read().strip()
     if not mach:
         raise ProcessorAutodetectError, "cannot run 'uname -m'"
     try:
         return {'i386': 'i386',
                 'i486': 'i386',
                 'i586': 'i386',
                 'i686': 'i386',
                 'i86pc': 'i386',    # Solaris/Intel
                 'x86':   'i386',    # Apple
                 }[mach]
     except KeyError:
         raise ProcessorAutodetectError, "unsupported processor '%s'" % mach
     
#-------------------------------------------------------------------------------


# This is borrowed from django's setup tools
def fullsplit(path, result=None):
    """
    Split a pathname into components (the opposite of os.path.join) in a
    platform-neutral way.
    """
    if result is None:
        result = []
    head, tail = os.path.split(path)
    if head == '':
        return [tail] + result
    if head == path:
        return result
    return fullsplit(head, [tail] + result)

#-------------------------------------------------------------------------------


"""
Returns a list of all files matching the given file types.
"""
_file_types = ['.py']

def find_files(root_directory, file_types=_file_types):

    package_files = []

    for path, directories, files in os.walk( root_directory ):
        
        for f in files:
            
            path_parts = fullsplit( os.path.join(path, f) )

            path_parts.pop(0)

            this_file = '/'.join(path_parts)

            basename, extension = os.path.splitext( this_file )
            
            if extension in file_types:

                package_files.append(this_file)

    return package_files


#-------------------------------------------------------------------------------
class ExperimentModule(Extension):
    """
    A class that abstracts methods that detect flags and paths
    for the target machine in a machine independent way. 
    """
    def __init__(self, name=None, sources=None, outdir=None,
                 library_files=None, library_paths=None,
                 include_files=None, include_paths=None):


                
        self._library_files = library_files
        self._library_paths = library_paths

        self._include_files = include_files
        self._include_paths = include_paths

        self.outdir = outdir
        self.name = name
        self.sources = sources
        self.swig_opts = self.get_swig_opts()
        self.extra_compile_args = self.get_extra_compile_args()
        self.libraries = self.get_libraries()

        # determine arch flags if we are on a mac
        if sys.platform == "darwin":

            arch = autodetect()
            
            if arch == 'i386':

                os.environ['ARCHFLAGS'] = self.get_mac_arch_flags()

        Extension.__init__(self,
                           self.name,
                           sources=self.sources,
                           swig_opts=self.get_swig_opts(),
                           extra_compile_args=self.get_extra_compile_args(),
                           library_dirs=self.get_library_dirs(),
                           include_dirs=self.get_include_dirs(),
                           libraries=self.get_libraries()
                           )
        
#     def __del__(self):
#         # Need this in order to make sure the heccer base python module
#         # is copied to the same place as the build binary.
#         import shutil
#         shutil.copy2(os.path.join(os.environ['HOME'],
#                                   'neurospaces_project', 'heccer',
#                                   'source', 'snapshots', '0',
#                                   'glue', 'swig', 'python', 'heccer_base.py'),
#                      os.path.join('neurospaces', 'heccer'))
#         pdb.set_trace()
        
    def get_extra_compile_args(self):

        return []

    def get_swig_opts(self):

        include_dirs = self.get_include_dirs()

        opts = []

        opts.extend(["-I%s" % d for d in include_dirs])


        opts.append("-I%s" % os.path.join(home_dir,
                                          'neurospaces_project',
                                          'experiment',
                                          'source',
                                          'snapshots',
                                          '0',
                                          'glue',
                                          'swig',
                                          'python'
                                          ))
        
        opts.extend(["-outdir", self.outdir])

        return opts
    
    def get_library_dirs(self):

        library_dirs = []

        for lib_file in self._library_files:

            this_path = self._get_path(self._library_paths, lib_file)

            if this_path is None:

                raise Exception("Can't find library path for %s, can't build extension\n" % lib_file)

            else:

                if not this_path in library_dirs:

                    library_dirs.append(this_path)

                    
        return library_dirs


    def get_include_dirs(self):

        include_dirs = []

        for inc_file in self._include_files:

            this_path = self._get_path(self._include_paths, inc_file)

            if this_path is None:

                raise Exception("Can't find path to headers for %s, can't build extension\n", inc_file)

            else:

                if not this_path in include_dirs:

                    include_dirs.append(this_path)

        return include_dirs


    def _get_path(self, dirs, file):
        """
        helper method, picks which path the given file is in and returns it.
        None if not found. 
        """
        
        for d in dirs:

            full_path = os.path.join(d, file)
            
            if os.path.isfile(full_path):

                return d

        return None

    
    def _in_path(self, dirs, file):

        for d in dirs:

            full_path = os.path.join(d, file)
            
            if os.path.isfile(full_path):

                return True

        return False


    def get_libraries(self):

        return ["neurospacesread","experiment",
                "symbol_algorithms", "ncurses", "readline"]

    def get_mac_arch_flags(self):

        lib_dirs = self.get_library_dirs()
        
        libraries = self.get_libraries()

        exe_archs = self.get_mac_architecture(sys.executable)

        
        if exe_archs is None:

            return "-arch i386"

        num_exe_archs = len(exe_archs)
        
        if num_exe_archs == 1:

            return "-arch %s" % "".join(exe_archs)

        # if we have more than one arch in the executable then
        # we check the libraries to make sure we match
        # up the archs

        lib_archs = []
        
        for lib in libraries:

            for lib_dir in lib_dirs:

                lib_file = "%s/lib%s.a" % (lib_dir, lib)

                if os.path.isfile(lib_file):

                    archs = self.get_mac_architecture(lib_file)

                    lib_archs.append(tuple(archs))
        
                    break

        least_archs = None

        for arch in lib_archs:

            if least_archs is None:

                least_archs = arch

            else:

                if len(arch) < len(least_archs):

                    least_archs = arch

        intel_archs = ['i486', 'i586', 'i686']

        common_archs = []

        for a in least_archs:

            if a in exe_archs:

                common_archs.append(a)

        return "-arch %s" % " -arch ".join(common_archs)
                
                
    def get_mac_architecture(self, file):
        """
        Returns string identifiers for the architecures present in the
        given file.

        """
        lipo_output = getoutput("lipo -info %s" % file)

        if re.search("can't figure out the architecture type of", lipo_output) or re.search("can't open input file", lipo_output):
            
            return None

        else:
            
            arches = lipo_output.split(':')[-1]

            return arches.split()


#-------------------------------------------------------------------------------
NAME = _package_info.GetName()
VERSION = _package_info.GetVersion()
AUTHOR = cbi.__author__
AUTHOR_EMAIL = cbi.__email__
LICENSE = cbi.__license__
URL = "http://pypi.python.org/pypi/experiment"
DOWNLOAD_URL = "http://pypi.python.org/pypi/experiment"
DESCRIPTION="The Experiment package delivers a library of stimulation and recording objects."
LONG_DESCRIPTION=cbi.__description__

KEYWORDS="neuroscience neurosimulator simulator modeling GENESIS"

# Get strings from http://pypi.python.org/pypi?%3Aaction=list_classifiers
CLASSIFIERS = [
    'Development Status :: 3 - Alpha',
    'Environment :: Console',
    'Intended Audience :: End Users/Desktop',
    'Intended Audience :: Developers',
    'Intended Audience :: Science/Research',
    'License :: OSI Approved :: GNU General Public License (GPL)',
    'Operating System :: MacOS :: MacOS X',
    'Operating System :: POSIX :: Linux',
    'Programming Language :: Python :: 2.5',
    'Programming Language :: Python :: 2.6',
    'Topic :: Software Development :: Libraries :: Python Modules',
]


PACKAGE_FILES=find_files(os.path.join('experiment'))

OPTIONS={
    'sdist': {
        'formats': ['gztar','zip'],
        'force_manifest': True,
        },
    'bdist_mpkg': {
        'zipdist': True,

        }
    }

PLATFORMS=["Unix", "Lunix", "MacOS X"]

CMDCLASS = {
#    'build_ext': build_ext,
    'bdist_rpm': bdist_rpm,
#    'bdist_mpkg': bdist_mpkg,
#    'test': test,
    }

if sys.platform == "darwin": 
    CMDCLASS = {'install_data': osx_install_data} 
else: 
    CMDCLASS = {'install_data': install_data}


#-------------------------------------------------------------------------------


home_dir = ''

if os.getenv('USER') == 'root' or os.getenv('LOGNAME') == 'root':

    _user = os.getenv('SUDO_USER')

    if _user is None:

        _user = os.getenv('LOGNAME')
        
    home_dir = os.path.expanduser("~%s" % _user)

else:

    _home = os.getenv('HOME')

    if _home is None:

        _home = os.getenv('USERPROFILE')
    
    home_dir = _home



_package_dir = os.path.join(home_dir,
                             'neurospaces_project',
                             'experiment',
                             'source',
                             'snapshots',
                             '0'
                             )

_model_container_package_dir = os.path.join(home_dir,
                                              'neurospaces_project',
                                              'model-container',
                                              'source',
                                              'snapshots',
                                              '0'
                                              )

_library_files = ["libexperiment.a", "libneurospacesread.a", "libsymbol_algorithms.a"]
_library_paths = [_package_dir,
                  _model_container_package_dir,
                  os.path.join(_model_container_package_dir, 'algorithms', 'symbol'),
                  "../../..",
                  "/usr/local/lib/model-container"]

#_include_files = [] #not needed
_include_paths = [_package_dir,
                  _model_container_package_dir,
                  "../../..",
                  "/usr/local/neurospaces/instrumentor"]

try:
    
    pulsegen_module=ExperimentModule(name='experiment._pulsegen_base',
                                     sources=['pulsegen.i'],
                                     outdir=os.path.join('experiment'),
                                     library_paths=_library_paths,
                                     library_files=_library_files,
                                     include_paths=_include_paths,
                                     include_files=['experiment/pulsegen.h'])
except Exception, e:

    sys.exit("Error creating pulsegen: %s" % e)



try:
    
    output_module=ExperimentModule(name='experiment._output_base',
                                   sources=['output.i'],
                                   outdir=os.path.join('experiment'),
                                   library_paths=_library_paths,
                                   library_files=_library_files,
                                   include_paths=_include_paths,
                                   include_files=['experiment/output.h'])
except Exception, e:

    sys.exit("Error creating output: %s" % e)



try:
    
    perfectclamp_module=ExperimentModule(name='experiment._perfectclamp_base',
                                         sources=['perfectclamp.i'],
                                         outdir=os.path.join('experiment'),
                                         library_paths=_library_paths,
                                         library_files=_library_files,
                                         include_paths=_include_paths,
                                         include_files=['experiment/perfectclamp.h'])

except Exception, e:

    sys.exit("Error creating perfectclamp: %s" % e)


EXT_MODULES=[
    pulsegen_module,
    output_module,
    perfectclamp_module,
    ]

#-------------------------------------------------------------------------------
setup(
    name=NAME,
    version=VERSION,
    author=AUTHOR,
    author_email=AUTHOR_EMAIL,
    cmdclass=CMDCLASS,
#    data_files=DATA_FILES,
    description=DESCRIPTION,
    long_description=LONG_DESCRIPTION,
    ext_modules=EXT_MODULES,
    license=LICENSE,
    keywords=KEYWORDS,
    url=URL,
    packages=['experiment'],
    package_data={'experiment' : PACKAGE_FILES,
                  'experiment' : [
                      os.path.join('experiment','output_base.py'),
                      os.path.join('experiment','pulsegen_base.py'),
                      os.path.join('experiment','perfectclamp_base.py'),
                      ],                  
                  },
    classifiers=CLASSIFIERS,
    options=OPTIONS,
    platforms=PLATFORMS,
)

