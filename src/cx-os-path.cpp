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
CX_CONSTRUCTOR4(PathBase<T>::PathBase, :_rootNameIndex(npos),
                                        _rootDirIndex(npos),
                                        _relativePathIndex(npos),
                                        _filenameIndex(npos) )

  _path.clear();

CX_ENDMETHOD



template <class T>
CX_CONSTRUCTOR(PathBase<T>::PathBase, :PathBase<T>::PathBase(),
                                                      string const& p)

  _initialize(p);

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(string PathBase<T>::str)

  string text("");

  // we need to construct "RootPath().str() + RelativePath().str()",
  // although we can't do exactly that because of infinite recursion
  if (!_path.empty())
  {
    for (size_t i=0; i<_path.size(); ++i)
    {
      if (!text.empty())
        if (!IsSeparator(text.back()) && (i != _rootDirIndex))
          text += PreferredSeparator();

      if (text.empty() || (i != _filenameIndex) || (_path.at(i) != dot()))
        text += _path.at(i);
    }
  }

  CX_RETURN(text);

CX_ENDMETHOD



template <class T>
CX_METHOD(string PathBase<T>::dot)

  CX_RETURN(string("."));

CX_ENDMETHOD



template <class T>
CX_METHOD(string PathBase<T>::dotdot)

  CX_RETURN(string(".."));

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(bool PathBase<T>::IsSeparator, char c)

  CX_RETURN((_separators().find(c) != std::string::npos));

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(char PathBase<T>::PreferredSeparator)

  CX_RETURN(_separators().at(0));

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(bool PathBase<T>::IsValid)

  if (_path.empty())
    CX_RETURN(false);

  CX_RETURN(_isValid());

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(bool PathBase<T>::IsEmpty)

  CX_RETURN(_path.empty());

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(bool PathBase<T>::IsAbsolute)

  if (_path.empty())
    CX_RETURN(false);  // empty path is neither absolute nor relative

  if (!RootDirectory().empty())
    CX_RETURN(true);

  CX_RETURN(false);

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(bool PathBase<T>::IsRelative)

  if (_path.empty())
    CX_RETURN(false);  // empty path is neither absolute nor relative

  CX_RETURN(!IsAbsolute());

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(string PathBase<T>::Stem)

  string filename = Filename().str();

  if (filename == dotdot())
    CX_RETURN(filename);

  size_t last = _getExtensionPos(filename);
  if ((last == 0) || (last == string::npos))
    CX_RETURN(filename);

  CX_RETURN(filename.substr(0, last));

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(string PathBase<T>::Extension)

  string filename = Filename().str();
  size_t last = _getExtensionPos(filename);
  if ((last == 0) || (last == string::npos))
    CX_RETURN("");

  // "the dot is included in the return value so that it is possible to
  // distinguish between no extension and an empty extension" -- boost
  CX_RETURN(filename.substr(last, string::npos));

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(T PathBase<T>::Filename)

  CX_RETURN(_component(_filenameIndex));

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(T PathBase<T>::RootName)

  CX_RETURN(_component(_rootNameIndex));

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(T PathBase<T>::RootDirectory)

  CX_RETURN(_component(_rootDirIndex));

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(T PathBase<T>::RootPath)

  CX_RETURN(RootName() / RootDirectory());

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(T PathBase<T>::ParentPath)

  if (empty() || (cbegin() == --cend()))
    CX_RETURN(T());

  T pp;
  for (auto p = cbegin(); p != --cend(); ++p)
    pp /= *p;

  CX_RETURN(pp);

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(T PathBase<T>::RelativePath)

  // construct a T comprised of everything that comes
  // after the root-directory

  // special cases: root-dir and root-naem have no relative-path
  if (_filenameIndex == _rootNameIndex)
    CX_RETURN(T(""));

  if (_filenameIndex == _rootDirIndex)
    CX_RETURN(T(""));

  size_t first = (_relativePathIndex != npos) ? _relativePathIndex
                                              : _filenameIndex;

  if (empty() || (first == npos))
    CX_RETURN(T(""));

  T rel("");
  for (size_t i = first; i<size(); i++)
    rel /= _path.at(i);

  CX_RETURN(rel);

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(bool PathBase<T>::HasRootPath)

  CX_RETURN(HasRootName() || HasRootDirectory());

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(bool PathBase<T>::HasRootName)

  CX_RETURN((_rootNameIndex != PathBase<T>::npos));

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(bool PathBase<T>::HasRootDirectory)

  CX_RETURN((_rootDirIndex != PathBase<T>::npos));

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(T PathBase<T>::LexicallyNormal)

  // a normal-form path is one with no redundant current directory (dot)
  // or parent directory (dot-dot) elements. The normal form for an
  // empty path is an empty path. The normal form for a path ending in
  // a directory-separator that is not the root directory is the same
  // path with a current directory (dot) element appended.

  T normal("");
  if (empty()) CX_RETURN(normal);

  bool first = true;
  T relative(RelativePath());
  for (auto &d : relative)
  {
    if (d == dotdot())
    {
      if (first)
      {
        // an initial '..' in the relative-path is only meaningful
        // when 'this' does not contain a root-directory
        if (!HasRootDirectory())
          normal /= d;
      }
      else
        normal.RemoveFilename();
    }
    else if (d == dot())
    {
      // only meaningful at the end of a normalized path
      if (&d == &relative.back())
        normal /= d;
    }
    else
    {
      first = false;
      normal /= d;
    }
  }
  normal = RootPath() / normal;

  if (normal.empty())
    normal += dot();

  CX_RETURN(normal);

CX_ENDMETHOD



// TODO: cribbed from BOOST 1_62_0
namespace detail
{
// C++14 provides a mismatch() algorithm with four iterator arguments,
// but earlier standard libraries didn't, so provide this needed
// functionality.
template <class T>
  std::pair<typename T::const_iterator, typename T::const_iterator>
  mismatch(typename T::const_iterator it1, typename T::const_iterator it1end,
      typename T::const_iterator it2, typename T::const_iterator it2end)
  {
    for (; it1 != it1end && it2 != it2end && *it1 == *it2;)
    {
      ++it1;
      ++it2;
    }
    CX_RETURN(std::make_pair(it1, it2));
  }
}



template <class T>
CX_CONSTMETHOD(T PathBase<T>::LexicallyRelative, T const& base)

  // if base is a prefix of *this, returns *this with base removed.
  // if base == *this, return dot().

  T relative;
  auto dir = detail::mismatch<T>(cbegin(), cend(), base.cbegin(), base.cend());

  if ((std::get<0>(dir) == cbegin()) || (std::get<1>(dir) == base.cbegin()))
    CX_RETURN(T(""));

  if ((std::get<0>(dir) == cend()) && (std::get<1>(dir) == base.cend()))
    CX_RETURN(T(dot()));

  for (auto& elem = std::get<0>(dir); elem != cend(); ++elem)
    relative /= *elem;

  CX_RETURN(relative);

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(T PathBase<T>::LexicallyAbsolute, T const& base)

  if (!base.IsAbsolute())
    CX_THROW( CX::OS::Exception,
              CX::OS::Error::BADPATH,
              "Cannot use non-absolute path '%s' to absolutize "
              "path '%s'.",
              base.str().c_str(), str().c_str() );

  T absolute;

  if (HasRootName())  // e.g., a network-name or drive-letter
  {
    if (HasRootDirectory())
      absolute = *this;
    else
    {
      if (RootName() != base.RootName())
        CX_THROW( CX::OS::Exception,
                  CX::OS::Error::BADPATH,
                  "Cannot absolutize a path with root-name '%s' "
                  "using a base path with root-name '%s'.",
                  RootName().str().c_str(),
                  base.RootName().str().c_str() );

      absolute = RootName() / base.RootDirectory()
                            / base.RelativePath()
                            / RelativePath();
    }
  }
  else
  {
    if (HasRootDirectory())
      absolute = base.RootName() / *this;
    else
      absolute = base / *this;
  }

  CX_RETURN(absolute);

CX_ENDMETHOD



template <class T>
CX_METHOD(PathBase<T>& PathBase<T>::operator/=, T const& p)

  if (p.HasRootName() && this->HasRootName())
    CX_THROW( CX::OS::Exception,
              CX::OS::Error::BADPATH,
              "Argument 'p' to operator/=() cannot have a root-name." );

  string lhs = str();
  string rhs = p.str();
  if ( !lhs.empty() && !rhs.empty()
      && !IsSeparator(lhs.back())
      && !IsSeparator(rhs.front()) )
  {
    lhs += PreferredSeparator();
  }
  lhs += rhs;
  _initialize(lhs);
  CX_RETURNREF(*this);

CX_ENDMETHOD



template <class T>
CX_METHOD(PathBase<T>& PathBase<T>::operator+=, T const& rhs)

  string lhs = str() + rhs.str();
  _initialize(lhs);
  CX_RETURNREF(*this);

CX_ENDMETHOD



template <class T>
CX_METHOD(void PathBase<T>::RemoveFilename)

  _path.pop_back();
  Normalize();

CX_ENDMETHOD



template <class T>
CX_METHOD(void PathBase<T>::RemoveExtension)

  string filename = Filename().str();
  size_t pos = _getExtensionPos(filename);
  if (pos != string::npos)
  {
    filename.erase(pos, string::npos);
    RemoveFilename();
    *this /= filename;
  }

CX_ENDMETHOD



template <class T>
CX_METHOD(void PathBase<T>::Normalize)

  *this = LexicallyNormal();

CX_ENDMETHOD



template <class T>
CX_METHOD(void PathBase<T>::Clear)

  _initialize("");

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(string PathBase<T>::_component_str, size_t index)

  if (index != PathBase<T>::npos)
    CX_RETURN(_path.at(index));

  CX_RETURN("");

CX_ENDMETHOD



template<class T>
CX_CONSTMETHOD(T PathBase<T>::_component, size_t index)

  CX_RETURN(T(_component_str(index)));

CX_ENDMETHOD



template <class T>
CX_METHOD(void PathBase<T>::_initialize, string const& p)

  _path.clear();

  string temp = p;
  _initRootName(&temp);

  // we cull after _initRootName(), because some root-names
  // legitimately have consecutive separators
  _cullRedundantSeparators(&temp);

  _initRootDirectory(&temp);
  _initRelativePath(&temp);
  _initFilename(&temp);

CX_ENDMETHOD



template <class T>
CX_METHOD(void PathBase<T>::_initRootDirectory, string* text)

  // for "C:\foo\bar\baz", the root-dir would be "\"
  // for "//net/foo/bar/baz", the root-name woudl be "/"
  assert(_path.size() <= 1);

  _rootDirIndex = PathBase<T>::npos;
  if (text->size() && IsSeparator(text->at(0)))
  {
    _rootDirIndex = _path.size();
    _path.push_back(std::string(1, PreferredSeparator()));
    text->erase(0,1);
  }

CX_ENDMETHOD



template <class T>
CX_METHOD(void PathBase<T>::_initRelativePath, string* text)

  // NOTE: For printing purposes, relative-path includes the filename!

  // for "C:\foo\bar", the relative-path would be "foo\bar"
  // for "//net/foo/bar/", the relative-path woudl be "foo/bar/"
  //                       (and the filename would be ".")

  _relativePathIndex = PathBase<T>::npos;

  size_t size = text->length();
  size_t next = text->find_first_of(_separators());

  // if there's at least one separator remaining, then we have
  // a relative path
  if (next != string::npos)
    _relativePathIndex = _path.size();

  while ( (next != string::npos) && (next <= size-1) )
  {
    _path.push_back(text->substr(0, next));
    text->erase(0, next+1);
    size -= next+1;
    next = text->find_first_of(_separators());
  }

  // if we ended on a separator, then there is an implied '.' at the
  // end of the path (which will be consumed later as the filename)
  if (!size && (_relativePathIndex != PathBase<T>::npos))
    *text += ".";

CX_ENDMETHOD



template <class T>
CX_METHOD(void PathBase<T>::_initFilename, string* text)

  // if 'text' is empty at this point, then filename aliases either
  // root-name or root-dir, otherwise it is empty
  if (text->empty() && (_relativePathIndex == npos))
  {
    if (_rootDirIndex == npos)
      if (_rootNameIndex == npos)
        _filenameIndex = npos;
      else
        _filenameIndex = _rootNameIndex;
    else
      _filenameIndex = _rootDirIndex;
    CX_RETURNVOID;
  }

  assert(!text->empty());  // we guarded against this elsewhere

  // anything that remains will be the filename, and its stem
  // will be distinguished from its extension on a JIT-basis
  _filenameIndex = _path.size();
  _path.push_back(*text);
  text->clear();

CX_ENDMETHOD



template <class T>
CX_METHOD(void PathBase<T>::_cullRedundantSeparators, string* text)

  size_t size = text->size();
  size_t next = 0;
  size_t last;
  do
  {
    next = text->find_first_of(_separators(), next);
    if (next != string::npos)
    {
      last = text->find_first_not_of(_separators(), next);
      last = (last != string::npos) ? last-1 : size-1;
      if (last-next)
      {
        text->replace(next, last-next+1, std::string(1, PreferredSeparator()));
        size -= (last-next);
      }
      next = (next < size-1) ? next+1 : string::npos;
    }
  } while (next != string::npos);

CX_ENDMETHOD



template <class T>
CX_CONSTMETHOD(size_t PathBase<T>::_getExtensionPos,  string const& name)

  if ((name == dot()) || (name == dotdot()))
    CX_RETURN(string::npos);

  size_t last = name.rfind(dot());
  CX_RETURN(last);

CX_ENDMETHOD


template class PathBase<Path>;


// ==================================================================


CX_FUNCTION(static int _lexcompare, Path const& lhs, Path const& rhs)

  size_t lsize = lhs.size();
  size_t rsize = rhs.size();

  for (size_t e=0; e < CX_MAX(lsize, rsize); e++)
  {
    if (lsize <= e) CX_RETURN(-1);
    if (rsize <= e) CX_RETURN(+1);

    std::string lelem = lhs.at(e);
    std::string relem = rhs.at(e);
    if (lelem < relem) CX_RETURN(-1);
    if (lelem > relem) CX_RETURN(+1);
  }

  CX_RETURN(0);

CX_ENDFUNCTION



CX_FUNCTION(bool operator <, Path const& lhs, Path const& rhs)

  CX_RETURN(_lexcompare(lhs, rhs) < 0);

CX_ENDFUNCTION



CX_FUNCTION(bool operator >,  Path const& lhs, Path const& rhs)

  CX_RETURN(_lexcompare(lhs, rhs) > 0);

CX_ENDFUNCTION



CX_FUNCTION(bool operator ==, Path const& lhs, Path const& rhs)

  CX_RETURN(_lexcompare(lhs, rhs) == 0);

CX_ENDFUNCTION



CX_FUNCTION(bool operator <=, Path const& lhs, Path const& rhs)

  CX_RETURN(_lexcompare(lhs, rhs) <= 0);

CX_ENDFUNCTION



CX_FUNCTION(bool operator >=, Path const& lhs, Path const& rhs)

  CX_RETURN(_lexcompare(lhs, rhs) >= 0);

CX_ENDFUNCTION



CX_FUNCTION(bool operator !=, Path const& lhs, Path const& rhs)

  CX_RETURN(_lexcompare(lhs, rhs) != 0);

CX_ENDFUNCTION



CX_FUNCTION(Path operator /, Path const& lhs, Path const& rhs)

  Path temp = lhs;
  temp /= rhs;
  CX_RETURN(temp);

CX_ENDFUNCTION



CX_FUNCTION(Path operator +, Path const& lhs, Path const& rhs)

  Path temp = lhs;
  temp += rhs;
  CX_RETURN(temp);

CX_ENDFUNCTION


} // namespace 'OS'
} // namespace 'CX'
