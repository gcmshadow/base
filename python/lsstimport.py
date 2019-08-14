#! env python

#
# LSST Data Management System
# Copyright 2008, 2009, 2010 LSST Corporation.
# Copyright 2015 AURA/LSST.
#
# This product includes software developed by the
# LSST Project (http://www.lsst.org/).
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the LSST License Statement and
# the GNU General Public License along with this program.  If not,
# see <http://www.lsstcorp.org/LegalNotices/>.
#

"""Configure Python library loader to support LSST shared libraries."""

__all__ = []

import sys

try:
    import lsstcppimport  # noqa F401
except ImportError:
    # The lsstcppimport may have failed because we're inside Scons.
    # If we are, then don't worry about it
    try:
        import SCons.Script  # noqa F401
    # If we're not, then
    #   a) we will get an ImportError trying to import SCons.Script
    #   b) and will know that the first ImportError really is a problem
    #        and we should let the user know.
    except ImportError:
        print(
            "Could not import lsstcppimport;"
            " please ensure the base package has been built (not just setup).\n",
            file=sys.stderr)
