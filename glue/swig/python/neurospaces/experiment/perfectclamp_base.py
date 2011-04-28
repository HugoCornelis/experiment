# This file was automatically generated by SWIG (http://www.swig.org).
# Version 1.3.31
#
# Don't modify this file, modify the SWIG interface instead.
# This file is compatible with both classic and new-style classes.

import _perfectclamp_base
import new
new_instancemethod = new.instancemethod
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'PySwigObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static) or hasattr(self,name):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0
del types


new_pdouble = _perfectclamp_base.new_pdouble
copy_pdouble = _perfectclamp_base.copy_pdouble
delete_pdouble = _perfectclamp_base.delete_pdouble
pdouble_assign = _perfectclamp_base.pdouble_assign
pdouble_value = _perfectclamp_base.pdouble_value
PerfectClampAddVariable = _perfectclamp_base.PerfectClampAddVariable
PerfectClampSingleStep = _perfectclamp_base.PerfectClampSingleStep
PerfectClampFinish = _perfectclamp_base.PerfectClampFinish
PerfectClampInitiate = _perfectclamp_base.PerfectClampInitiate
PerfectClampNew = _perfectclamp_base.PerfectClampNew
PerfectClampSetFields = _perfectclamp_base.PerfectClampSetFields
class simobj_PerfectClamp(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, simobj_PerfectClamp, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, simobj_PerfectClamp, name)
    __repr__ = _swig_repr
    __swig_setmethods__["pcName"] = _perfectclamp_base.simobj_PerfectClamp_pcName_set
    __swig_getmethods__["pcName"] = _perfectclamp_base.simobj_PerfectClamp_pcName_get
    if _newclass:pcName = _swig_property(_perfectclamp_base.simobj_PerfectClamp_pcName_get, _perfectclamp_base.simobj_PerfectClamp_pcName_set)
    __swig_setmethods__["iClampsActive"] = _perfectclamp_base.simobj_PerfectClamp_iClampsActive_set
    __swig_getmethods__["iClampsActive"] = _perfectclamp_base.simobj_PerfectClamp_iClampsActive_get
    if _newclass:iClampsActive = _swig_property(_perfectclamp_base.simobj_PerfectClamp_iClampsActive_get, _perfectclamp_base.simobj_PerfectClamp_iClampsActive_set)
    __swig_setmethods__["pdVoltage"] = _perfectclamp_base.simobj_PerfectClamp_pdVoltage_set
    __swig_getmethods__["pdVoltage"] = _perfectclamp_base.simobj_PerfectClamp_pdVoltage_get
    if _newclass:pdVoltage = _swig_property(_perfectclamp_base.simobj_PerfectClamp_pdVoltage_get, _perfectclamp_base.simobj_PerfectClamp_pdVoltage_set)
    __swig_setmethods__["dCommand"] = _perfectclamp_base.simobj_PerfectClamp_dCommand_set
    __swig_getmethods__["dCommand"] = _perfectclamp_base.simobj_PerfectClamp_dCommand_get
    if _newclass:dCommand = _swig_property(_perfectclamp_base.simobj_PerfectClamp_dCommand_get, _perfectclamp_base.simobj_PerfectClamp_dCommand_set)
    __swig_setmethods__["pcFilename"] = _perfectclamp_base.simobj_PerfectClamp_pcFilename_set
    __swig_getmethods__["pcFilename"] = _perfectclamp_base.simobj_PerfectClamp_pcFilename_get
    if _newclass:pcFilename = _swig_property(_perfectclamp_base.simobj_PerfectClamp_pcFilename_get, _perfectclamp_base.simobj_PerfectClamp_pcFilename_set)
    __swig_setmethods__["pfile"] = _perfectclamp_base.simobj_PerfectClamp_pfile_set
    __swig_getmethods__["pfile"] = _perfectclamp_base.simobj_PerfectClamp_pfile_get
    if _newclass:pfile = _swig_property(_perfectclamp_base.simobj_PerfectClamp_pfile_get, _perfectclamp_base.simobj_PerfectClamp_pfile_set)
    def __init__(self, *args): 
        this = _perfectclamp_base.new_simobj_PerfectClamp(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _perfectclamp_base.delete_simobj_PerfectClamp
    __del__ = lambda self : None;
simobj_PerfectClamp_swigregister = _perfectclamp_base.simobj_PerfectClamp_swigregister
simobj_PerfectClamp_swigregister(simobj_PerfectClamp)



