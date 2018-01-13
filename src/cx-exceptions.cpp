// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 1999-2002,2013-2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#include <typeinfo>

#include <stdio.h>
#include <stdarg.h>


// this disables tracing for exceptions.
#undef CX_TRACING

#include "cx-types.hpp"
#include "cx-exceptions.hpp"


char const * CX::thrown_message = "{{{ Exception thrown in '%s' }}}\n";
char const * CX::caught_message = "{{{ Exception caught in '%s' }}}\n";
char const * CX::trace_reset_message = "{{{ Tracing reset }}}\n";

#define X(v,str) { CX::Error::v, "CXError::"#v },
std::map<CX::Error, const char*> CX::ErrorNameMap =
{
  CX_EXCEPTIONS
};
#undef X

#define X(v,str) { CX::Error::v, str },
std::map<CX::Error, const char*> CX::ErrorStringMap =
{
  CX_EXCEPTIONS
};
#undef X


CX::BaseException::BaseException( const char* who,
                                  const char* where,
                                  U32 what)
{
  _who = who;
  _what = what;

  SetWhere(where);

  _reason = nullptr;
  _reasonbuffer[0] = '\x0';
}


void CX::BaseException::SetWhere(const char* where)
{
  _where = nullptr;
  _wherebuffer[0] = '\x0';

  if (where)
  {
    // strip off path to filename
    const char* find = where + strlen(where);
    do {
      --find;
    } while ((find >= where) && ((*find != '/') && (*find != '\\')));
    _where = _wherebuffer;
    strncpy(_wherebuffer, find+1, CX_EXCEPTIONSIZE-1);
  }
}


void CX::BaseException::SetReason(const char* format, va_list pArgs)
{
  if (format)
  {
    vsnprintf(_reasonbuffer, CX_EXCEPTIONSIZE-1, format, pArgs);

    _reason = _reasonbuffer;
    _reasonbuffer[CX_EXCEPTIONSIZE-1] = '\x0';  // ...just in case
  }
}


#undef CX_DEMANGLE
#if 0
#ifdef __GNUC__
#include <cxxabi.h> // abi::__cxa_demangle()

#define CX_DEMANGLE

static const char* CX::_malloc_demangled_name(onst char* name)
{
#ifdef __GNUC__
  int status;
  name = abi::__cxa_demangle(name, 0, 0, &status);
#else
  name = strdup(name);
#endif
  return name;
}
#endif
#endif


void CX::BaseException::StdError() const
{
#ifdef CX_DEMANGLE
  const char* name = typeid(*this).name();
  name = CX::_malloc_demangled_name(name);
#endif

  CX_ERROROUT("UNHANDLED EXCEPTION '%s':\n", Name());
  CX_ERROROUT("Who:     %s\n",  Who());
  CX_ERROROUT("Where:   %s\n",  Where());
  CX_ERROROUT("Why:     '%s'\n", Why());
  CX_ERROROUT("Reason:  '%s'\n", Reason());

#ifdef CX_DEMANGLE
  free(const_cast<char*>(name));
#endif
}

