/*
 * LSST Data Management System
 * Copyright 2008-2016  AURA/LSST.
 *
 * This product includes software developed by the
 * LSST Project (http://www.lsst.org/).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the LSST License Statement and
 * the GNU General Public License along with this program.  If not,
 * see <https://www.lsstcorp.org/LegalNotices/>.
 */

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include "lsst/base/versions.h"

namespace py = pybind11;

PYBIND11_PLUGIN(_versions) {
    py::module mod("_versions", "Access to the classes from the versions library");

    mod.def("getRuntimeVersions", &lsst::base::getRuntimeVersions);
    mod.def("getCfitsioVersion", &lsst::base::getCfitsioVersion);
    mod.def("getFftwVersion", &lsst::base::getFftwVersion);
    mod.def("getWcslibVersion", &lsst::base::getWcslibVersion);
    mod.def("getGslVersion", &lsst::base::getGslVersion);

    return mod.ptr();
}