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


CX_CONSTRUCTOR(Path_Linux::Path_Linux,:PathBase<Path_Linux>())
CX_ENDMETHOD



CX_CONSTRUCTOR(Path_Linux::Path_Linux, :PathBase<Path_Linux>(p),
                                                      string const& p)
CX_ENDMETHOD



CX_CONSTRUCTOR(Path_Linux::Path_Linux, :PathBase<Path_Linux>(that),
                                      PathBase<Path_Linux> const& that)
CX_ENDMETHOD



template <>
CX_CONSTMETHOD(bool PathBase<Path_Linux>::_isValid)

  // pretty much anything is valid on Unix filesystems, except
  // the NUL character.  But there are certain characters that
  // must be quoted, such as spaces, <, >, |, \, and sometimes
  // :, (, ), &, ;, #, as well as wildcards such as ? and *.
  //
  // our str() and c_str() methods are not yet sophisticated
  // enough to echo those in an escaped manner, though, so here
  // we will treat them as illegal until we add that functionality

  string illegals(":()&;%< >|\\?*\"'");
  for (auto const& p : _path)
  {
    size_t found = p.find_first_of(illegals);
    if (found != string::npos)
      return false;
  }

return true;

CX_ENDMETHOD



template <>
CX_CONSTMETHOD(string PathBase<Path_Linux>::_separators)

  return string("/\\");

CX_ENDMETHOD



template <>
CX_METHOD(void PathBase<Path_Linux>::_initRootName, string* text)

  _rootNameIndex = npos;

  // for Linux, we recognize '//foo' as a root-name (for networking)
  if ( (text->size() <= 2)  || !IsSeparator(text->at(0))
                            || !IsSeparator(text->at(1)) )
    return;

  // but more than two consecutive separators is disallowed
  if ( (text->size() >2) && IsSeparator(text->at(2)) )
    return;

  size_t last = 2;
  last = text->find_first_of(_separators(), last);
  last = (last != npos) ? last : text->size();
  _rootNameIndex = _path.size();
  _path.push_back(text->substr(0, last));
  text->erase(0,last);

CX_ENDMETHOD


} // namespace OS
} // namespace CX

