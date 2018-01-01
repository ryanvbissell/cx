#
# Copyright (c) 2016-2017, Ryan V. Bissell
# All rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
# See the enclosed "LICENSE" file for exact license terms.
#

.DEFAULT_GOAL:=static

define MF_HELPDOC
USAGE:  make <target> [flags]
SUPPORTED TARGETS:
	*   help              -- this message
	*   clean             -- erase cx-specific contents of CXOUT
	*   static (default)  -- build libcx.a
SUPPORTED FLAGS:
All of the usual (CPPFLAGS, CXXFLAGS, etc.) plus:
	*   MFDIR=<path>      -- use alternate make-forge (see docs)
	*   CROSS_COMPILE=... -- target triplet of toolchain to use
	*   CPPONLY=1         -- preprocess only
	*   CXTRACE=1         -- enable tracing output
	*   CXDEBUG=1         -- enable debug output
	*   CXALL=1           -- CXTRACE=1 and CXDEBUG=1
	*   DEBUG=1           -- build debug library
	*   PROFILE=1         -- build profile-able library
	*   CXOUT=<path>      -- location to place built binaries (default: ./out)
	*   WITH='foo bar'    -- see below
	*   VERBOSE=1         -- verbose GNUMakefile output
	*   TEST=1            -- dry-run of recipes for <target>
If you only need certain components in your private build of libcx.a,
you can exclude everything else by listing your needs in WITH, each
component name separated by spaces.  Recognized components are:
	*   args
	*   curses
	*   endian
	*   os
The 'trace' & 'exceptions' components are always included, as they are
used by all other components.  So, if they are all you want built,
the define WITH to be the empty string.  (Undefined WITH will cause
every component to be included in the build.)
endef


# this is what clients of CX will use to influence where
# the CX build system deposits the results of its build
CXOUT:=$(CURDIR)/out

# however, CX uses make-forge for its build system, and
# make-forge expects MFOUT to be set, for that purpose.
# override MFOUT:=$(CXOUT)

# initialize the build system
MF_PROJECT_DIR:=$(CURDIR)
MFDIR:=$(MF_PROJECT_DIR)/_forge
include $(MFDIR)/make-forge

override CXDIR:=$(CURDIR)
include $(CXDIR)/GNUmakefile.MF

