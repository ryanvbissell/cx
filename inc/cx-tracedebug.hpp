// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 1999-2002,2013-2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef CX_TRACEDEBUG_HPP
#define CX_TRACEDEBUG_HPP

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>  // abort()
#include <string.h>

#include "cx-types.hpp"

namespace CX
{
  bool is_enabled();

  U64 get_tracelevel();
  void set_tracelevel(U64 level);

  void shift_in();
  void shift_out();
  bool is_section_active(const char* section);

  void set_debugfile(FILE *file);
  void set_errorfile(FILE *file);
  void flush();

  void debugout(char const* format, ...);
  void warning(bool test, char const* format, ...);
  void topicout(char const* topic, char const* format, ...);
  void errorout(bool test, char const* format, ...);
  void traceout(char const* section, char const* format, ...);

#ifdef CX_TESTING
  char const *get_topicenv();
  char const *get_traceenv();
  char const *get_tracefile();
#endif

  char const* get_assert_message();

} // namespace 'CX'

#define CX_DIV0ASSERT(expr) (1/!!(expr))


// this global constant is set to zero, to cause a DIV0ASSERT
// when 'cx_traceflag' is not defined as a local and set to
// a non-zero value.  This is a way of ensuring that the
// CX_TRACExxx macros are never invoked in functions that
// don't use CX_METHOD.
const U64 cx_traceflag=0;

#if CX_OPT_TRACING
  // when tracing is turned on, this macro defines the local variables
  // necessary for managing the trace feature
  #define CX_TRACE_STACK                                              \
          U64 cx_traceflag=1;                                         \
          U64 cx_catchlevel=0;
#else
  #define CX_TRACE_STACK
#endif

// always defined
#define CX_ERROROUT(format_and_args...)                               \
        {                                                             \
          CX::errorout(true, format_and_args);                        \
        }

// predicated error message
#define CX_PERROROUT(test, format_and_args...)                        \
        {                                                             \
          CX::errorout((test), format_and_args);                      \
        }

#if CX_OPT_DEBUGOUT
  #define CX_DEBUGOUT(format_and_args...)                             \
          {                                                           \
            CX::debugout(format_and_args);                            \
          }
#else
  #define CX_DEBUGOUT(format_and_args...)
#endif

#if CX_OPT_DEBUGOUT
  #define CX_TOPICOUT(topic, format_and_args...)                      \
          {                                                           \
            CX::topicout(#topic, format_and_args);                    \
          }
#else
  #define CX_TOPICOUT(topic, format_and_args...)
#endif

#if CX_OPSYS == linux
#include <signal.h>
#define CX_BREAKPOINT raise(SIGINT)
#else
#define CX_BREAKPOINT
#endif

#define CX_ASSERT(expr, format_and_args...)                           \
{                                                                     \
  if (!(expr))                                                        \
  {                                                                   \
    CX_ERROROUT("\n!!! %s\n\n", CX::get_assert_message());            \
    CX_ERROROUT("    What:  assertion / debugger-trap\n");            \
    CX_ERROROUT("    Why:   " format_and_args);                       \
    CX_ERROROUT("\n");                                                \
    CX_ERROROUT("    How:   '%s'\n", #expr);                          \
    CX_ERROROUT("    Who:   '%s'\n", __PRETTY_FUNCTION__);            \
    CX_ERROROUT("    Where: '%s', line %d\n", __FILE__, __LINE__);    \
    CX::flush();                                                      \
    CX_BREAKPOINT;                                                    \
    abort();                                                          \
  }                                                                   \
}

#define CX_WARNING(format_and_args...)                                \
        {                                                             \
          CX::warning(true, format_and_args);                         \
        }                                                             \

// predicated warning
#define CX_PWARNING(test, format_and_args...)                         \
        {                                                             \
          CX::warning((test), format_and_args);                       \
        }

#if CX_OPT_TRACING
  #define CX_TRACEOUT(format_and_args...)                             \
    {                                                                 \
      CX::traceout(CX_TRACE_SECTION, format_and_args);                \
    }
#else
  #define CX_TRACEOUT(format_and_args...)
#endif

#ifndef CX_TRACE_SECTION
  #define CX_TRACE_SECTION ""
#endif

#if CX_OPT_TRACING
  #define CX_TRACE_SHIFTIN(text)                                      \
    if (CX::is_section_active(CX_TRACE_SECTION))                      \
    {                                                                 \
      CX_DIV0ASSERT(cx_traceflag); /* enforces use of CX_METHOD */    \
      CX_TRACEOUT(text);                                              \
      CX::shift_in();                                                 \
    }

  #define CX_TRACE_SHIFTOUT(...)                                      \
    if (CX::is_section_active(CX_TRACE_SECTION))                      \
    {                                                                 \
      CX_DIV0ASSERT(cx_traceflag); /* enforces use of CX_METHOD */    \
      CX::shift_out();                                                \
      CX_TRACEOUT(__VA_ARGS__);                                       \
    }
#else
  #define CX_TRACE_SHIFTIN(...)
  #define CX_TRACE_SHIFTOUT(...)
#endif


extern char const* cx_trace_methodname;
#if CX_OPT_TRACING
  #define CX_TRACE_PROLOGUE(name, args, decl)                         \
      CX_TRACE_STACK                                                  \
      char const* cx_trace_methodname = name;                         \
      CX_TRACE_SHIFTIN(">" name "(" args ") " decl "\n");
#else
  #define CX_TRACE_PROLOGUE(name, args, decl)
#endif

#define CX_DECLMETHOD(object_method, declarator, ...)                 \
  object_method(__VA_ARGS__) declarator                               \
  {                                                                   \
    CX_TRACE_PROLOGUE(#object_method, #__VA_ARGS__, #declarator)

#define CX_FUNCTION(function, ...)                                    \
  CX_DECLMETHOD(function,,__VA_ARGS__)

#define CX_METHOD(object_method, ...)                                 \
  CX_DECLMETHOD(object_method,,__VA_ARGS__)

#define CX_CONSTMETHOD(object_method, ...)                            \
  CX_DECLMETHOD(object_method, const, __VA_ARGS__)

#define CX_CONSTRUCTOR0(ctorname, ...)                                \
  CX_METHOD(ctorname, __VA_ARGS__)

#define CX_CONSTRUCTOR(ctorname, init1, ...)                          \
  ctorname(__VA_ARGS__) init1                                         \
  {                                                                   \
    CX_TRACE_PROLOGUE(#ctorname, #__VA_ARGS__,)

#define CX_CONSTRUCTOR2(ctorname, init1, init2, ...)                  \
  ctorname(__VA_ARGS__) init1, init2                                  \
  {                                                                   \
    CX_TRACE_PROLOGUE(#ctorname, #__VA_ARGS__,)

#define CX_CONSTRUCTOR3(ctorname, init1, init2, init3, ...)           \
  ctorname(__VA_ARGS__) init1, init2, init3                           \
  {                                                                   \
    CX_TRACE_PROLOGUE(#ctorname, #__VA_ARGS__,)

#define CX_CONSTRUCTOR4(ctorname, init1, init2, init3, init4, ...)    \
  ctorname(__VA_ARGS__) init1, init2, init3, init4                    \
  {                                                                   \
    CX_TRACE_PROLOGUE(#ctorname, #__VA_ARGS__,)

#define CX_CONSTRUCTOR5(ctorname,                                     \
                        init1, init2, init3, init4, init5, ...)       \
  ctorname(__VA_ARGS__) init1, init2, init3, init4, init5             \
  {                                                                   \
      CX_TRACE_PROLOGUE(#ctorname, #__VA_ARGS__,)

#define CX_CONSTRUCTOR6(ctorname,                                     \
                        init1, init2, init3, init4, init5, init6, ...)\
  ctorname(__VA_ARGS__) init1, init2, init3, init4, init5, init6      \
  {                                                                   \
    CX_TRACE_PROLOGUE(#ctorname, #__VA_ARGS__,)

#define CX_DESTRUCTOR(dtorname, ...)                                  \
  dtorname()                                                          \
  {                                                                   \
    CX_TRACE_PROLOGUE(#dtorname, #__VA_ARGS__,)


#if CX_OPT_TRACING
  #define CX_RETURNVOID                                               \
          do {                                                        \
            CX_DIV0ASSERT(cx_traceflag);                              \
            CX_TRACE_SHIFTOUT("<\n");                                 \
            CX::flush();                                              \
            return;                                                   \
          } while(0)

  #define CX_RETURNREF(...)                                           \
          do {                                                        \
            CX_DIV0ASSERT(cx_traceflag);                              \
            auto& foo =  __VA_ARGS__;                                 \
            CX_TRACE_SHIFTOUT("<\n");                                 \
            CX::flush();                                              \
            return foo;                                               \
          } while(0)

  #define CX_RETURN(...)                                              \
          do {                                                        \
            CX_DIV0ASSERT(cx_traceflag);                              \
            auto foo =  __VA_ARGS__;                                  \
            CX_TRACE_SHIFTOUT("<\n");                                 \
            CX::flush();                                              \
            return foo;                                               \
          } while(0)
#else
  #define CX_RETURNVOID return
  #define CX_RETURNREF(...) return __VA_ARGS__
  #define CX_RETURN(...) return __VA_ARGS__
#endif

#if CX_OPT_TRACING
  #define CX_ENDMETHOD                                                \
            CX_DIV0ASSERT(cx_traceflag);                              \
            CX_TRACE_SHIFTOUT("<\n");                                 \
          }
#else
  #define CX_ENDMETHOD }
#endif

#define CX_ENDFUNCTION CX_ENDMETHOD



#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <string>
namespace CX
{
  template <typename T>
  std::string get_typename();
}

#include <string>
#include <typeinfo>
#include <cxxabi.h>
template<typename T>
std::string CX::get_typename()
{
  std::string tname = typeid(T).name();
#if defined(__clang__) || defined(__GNUG__)
  int status;
  char *demangled_name = abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status);
  if (!status)
  {
    tname = demangled_name;
    std::free(demangled_name);
  }
#endif
  return tname;
}
#endif // __cplusplus

#endif  // CX_TRACEDEBUG_HPP

