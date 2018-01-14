// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "os"

#include "cx-os.hpp"
#include "cx-exceptions.hpp"

using std::string;

namespace CX {
namespace OS {


CX_CONSTRUCTOR(PathList_Linux::PathList_Linux,
                                        :PathListBase<PathList_Linux>())
CX_ENDMETHOD



CX_CONSTRUCTOR(PathList_Linux::PathList_Linux,
                              :PathListBase<PathList_Linux>(p),
                                                      string const& p)
CX_ENDMETHOD



CX_CONSTRUCTOR(PathList_Linux::PathList_Linux,
                              :PathListBase<PathList_Linux>(that),
                              PathListBase<PathList_Linux> const& that)
CX_ENDMETHOD



template <>
CX_METHOD(char PathListBase<PathList_Linux>::_delim)

  CX_RETURN(':');

CX_ENDMETHOD


} // namespace OS
} // namespace CX

