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

#include <assert.h>
#include <utility>

using std::string;

namespace CX {
namespace OS {


template <class T>
CX_CONSTRUCTOR0(PathListBase<T>::PathListBase)

  _list.clear();

CX_ENDMETHOD



template <class T>
CX_CONSTRUCTOR(PathListBase<T>::PathListBase,
                               :PathListBase<T>::PathListBase(),
                                                      string const& p)

  _initialize(p);

CX_ENDMETHOD



template <class T>
CX_METHOD(char PathListBase<T>::Delim)

  CX_RETURN(_delim());

CX_ENDMETHOD



template <class T>
CX_METHOD(bool PathListBase<T>::IsDelim, char c)

  CX_RETURN(c == _delim());

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(string PathListBase<T>::str)

  string text("");

  for (auto p = cbegin(); p != cend(); ++p)
  {
    text += (*p).str();
    if (p != cend()-1)
      text += _delim();
  }

  CX_RETURN(text);

CX_ENDMETHOD



template <class T>
CX_METHOD(PathListBase<T>& PathListBase<T>::operator+=, Path const& p)

  _list.push_back(p);

CX_ENDMETHOD



template <class T>
CX_METHOD(void PathListBase<T>::_initialize, string const& text)

  size_t prev;
  size_t next = string::npos;

  _list.clear();
  do
  {
    prev = next;
    next = text.find(_delim(), next+1);
    Path path(text.substr(prev+1, next-(prev+1)));
    _list.push_back(path);
  } while (next != string::npos);

CX_ENDMETHOD


template class PathListBase<PathList>;


// ====================================================================


CX_FUNCTION(PathList operator +, PathList const& lhs, Path const& rhs)

  PathList temp = lhs;
  temp += rhs;
  CX_RETURN(temp);

CX_ENDFUNCTION


} // namespace 'OS'
} // namespace 'CX'
