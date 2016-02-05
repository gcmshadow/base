// -*- lsst-c++ -*-

%define baseLib_DOCSTRING
"
Access to the lsst::base functions
"
%enddef

%feature("autodoc", "1");
%module(package="lsst.base", docstring=baseLib_DOCSTRING) baseLib

%{
#include "lsst/base/threads.h"
%}

%include "lsst/base/threads.h"
