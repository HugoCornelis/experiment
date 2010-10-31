#! /usr/bin/env python
"""
This is a module with library of functions
for testing the python bindings.
"""
import os
import sys
import pdb

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