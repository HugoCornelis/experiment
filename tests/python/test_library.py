#! /usr/bin/env python
"""
This is a module with library of functions
for testing the python bindings.
"""
import os
import sys
import pdb

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

def add_package_path(package, subdir=''):
    """
    Adds an import path to a python module in a project directory.
    """

    path = os.path.join(os.environ['HOME'],
                        'neurospaces_project',
                        package,
                        'source',
                        'snapshots',
                        '0',
                        'glue',
                        'swig',
                        'python')

    build_dir = os.path.join(path, 'build')

    python_build = ""
    parts = []

    if os.path.exists(build_dir):

        for curr_dir, directories, files in os.walk( build_dir ):

            if os.path.isfile( os.path.join( curr_dir, '__cbi__.py' )):

                parts = list(fullsplit(curr_dir))

                parts.pop() # remove the chemesis3 path
                parts.pop() # remove the neurospaces path

                python_build = os.path.join(os.sep, os.path.join(*parts))
                #sys.path.append(os.path.join(python_build, subdir))
                sys.path.insert(0, os.path.join(python_build, subdir))

                return

    # Add this path if we didn't find one previously
    sys.path.insert(0, os.path.join(path, subdir))
#    sys.path.append(os.path.join(path, subdir))



def print_simobj_pulsegen(spg):

    print "Name: %s" % spg.pcName
    print "Level1: %s" % spg.dLevel1
    print "Width1: %s" % spg.dWidth1
    print "Delay1: %s" % spg.dDelay1
    print "Level2: %s" % spg.dLevel2
    print "Width2: %s" % spg.dWidth2
    print "Delay2: %s" % spg.dDelay2
    print "BaseLevel: %s" % spg.dBaseLevel
    print "TriggerMode: %s" % spg.iTriggerMode


def print_simobj_perfectclamp(spc):

    print "Name: %s" % spc.pcName
    print "Clamps Active: %d" % spc.iClampsActive
#    print "Voltage: %s" % spc.pdVoltage
    print "Command: %.2f" % spc.dCommand
    print "Filename: %s" % spc.pcFilename
    
def print_pulsegen(pg):

    print "Name: %s" % pg.GetName()
    print "Level1: %s" % pg.GetLevel1()
    print "Width1: %s" % pg.GetWidth1()
    print "Delay1: %s" % pg.GetDelay1()
    print "Level2: %s" % pg.GetLevel2()
    print "Width2: %s" % pg.GetWidth2()
    print "Delay2: %s" % pg.GetDelay2()
    print "BaseLevel: %s" % pg.GetBaseLevel()
    print "TriggerMode: %s" % pg.GetTriggerMode()
