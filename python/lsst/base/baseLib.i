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
#include "lsst/base/versions.h"
%}

%include "lsst/base/threads.h"

// Convert output of getRuntimeVersions() to dict
%typemap(out) std::map<std::string,std::string> {
    $result = PyDict_New();
    for (auto&& pp : $1) {
        PyDict_SetItem($result, PyString_FromString(pp.first.c_str()),
                       PyString_FromString(pp.second.c_str()));
    }
}

%include "lsst/base/versions.h"
