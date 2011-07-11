import imp
import os
import pdb
import sys
from commands import getoutput

from distutils import log
from distutils.core import setup, Extension
from distutils.command.bdist_rpm import bdist_rpm
from distutils.command.install_data import install_data

#from setuptools import setup, find_packages

# import the cbi module. We use this since the check
# for the compiled swig nmc_base gives an error
# if we import from nmc.__cbi__
cbi = imp.load_source('__cbi__', os.path.join('neurospaces', 'experiment', '__cbi__.py'))


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


        if sys.platform == "darwin":

            arch = autodetect()
            
            if arch == 'i386':

                os.environ['ARCHFLAGS'] = "-arch i386"
                
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

        swig_opts = [ '-I../../..']

        if not self.outdir is None:

            swig_opts.extend(['-outdir', self.outdir])

        return swig_opts
    
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

        return ["neurospacesread",
                "symbol_algorithms", "ncurses", "readline"]

    def get_mac_architectures(self, file):
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
NAME = cbi.GetPackageName()
VERSION = cbi.GetVersion()
AUTHOR = cbi.__author__
AUTHOR_EMAIL = cbi.__email__
LICENSE = cbi.__license__
URL = cbi.__url__
DOWNLOAD_URL = cbi.__download_url__
DESCRIPTION="Heccer is a fast compartmental solver, that is based on hsolve of the GENESIS simulator."
LONG_DESCRIPTION=cbi.__description__

KEYWORDS="neuroscience neurosimulator simulator modeling GENESIS"

# Get strings from http://pypi.python.org/pypi?%3Aaction=list_classifiers
CLASSIFIERS = [
    'Development Status :: 0 - Alpha',
    'Environment :: Console',
    'Environment :: Desktop Application',
    'Intended Audience :: End Users/Desktop',
    'Intended Audience :: Developers',
    'Intended Audience :: Research',
    'Intended Audience :: Science',        
    'License :: OSI Approved :: GPL License',
    'Operating System :: MacOS :: MacOS X',
    'Operating System :: POSIX',
    'Programming Language :: Python',
    'Topic :: Research :: Neuroscience',
]

PACKAGE_FILES=find_files(os.path.join('neurospaces', 'experiment'))

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

PY_MODULES=['neurospaces.heccer']


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


home_dir = os.getenv('USERPROFILE') or os.getenv('HOME')

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
                  "../../..",
                  "/usr/local/lib/model-container"]

#_include_files = [] #not needed
_include_paths = [_package_dir,
                  _model_container_package_dir,
                  "../../..",
                  "/usr/local/neurospaces/instrumentor"]

pulsegen_module=ExperimentModule(name='neurospaces.experiment._pulsegen_base',
                                 sources=['pulsegen.i'],
                                 outdir=os.path.join('neurospaces','experiment'),
                                 library_paths=_library_paths,
                                 library_files=_library_files,
                                 include_paths=_include_paths,
                                 include_files=['experiment/pulsegen.h'])



output_module=ExperimentModule(name='neurospaces.experiment._output_base',
                               sources=['output.i'],
                               outdir=os.path.join('neurospaces','experiment'),
                               library_paths=_library_paths,
                               library_files=_library_files,
                               include_paths=_include_paths,
                               include_files=['experiment/output.h'])



perfectclamp_module=ExperimentModule(name='neurospaces.experiment._perfectclamp_base',
                                     sources=['perfectclamp.i'],
                                     outdir=os.path.join('neurospaces','experiment'),
                                     library_paths=_library_paths,
                                     library_files=_library_files,
                                     include_paths=_include_paths,
                                     include_files=['experiment/perfectclamp.h'])


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
    packages=['neurospaces',
              'neurospaces.experiment',
              ],
    package_data={'neurospaces' : [os.path.join('neurospaces','__init__.py')],
                  'neurospaces.experiment' : PACKAGE_FILES,
                  'neurospaces.experiment' : [
                      os.path.join('neurospaces', 'experiment','output_base.py'),
                      os.path.join('neurospaces', 'experiment','pulsegen_base.py'),
                      os.path.join('neurospaces', 'experiment','perfectclamp_base.py'),
                      ],                  
                  },
#     package_dir={'' : ''},
    classifiers=CLASSIFIERS,
    options=OPTIONS,
    platforms=PLATFORMS,
)
