// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 1999-2002,2013-2016 Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef CX_EXCEPTIONS_HPP
#define CX_EXCEPTIONS_HPP

#include <map>
#include <iostream>
#include <stdarg.h>

#include "cx-types.hpp"
#include "cx-hackery.hpp"
#include "cx-tracedebug.hpp"

#define CX_EXCEPTIONSIZE 1024

namespace CX
{
  extern const char* caught_message;
  extern const char* thrown_message;
  extern const char* trace_reset_message;

  class BaseException
  {
  public:
    BaseException(const char* who, const char* where, const U32 what);

    const char* Who() const     { return _who;    }
    const char* Where() const   { return _where;  }
    const char* Reason() const  { return _reason; }

    virtual const char* Why() const = 0;
    virtual const char* Name() const = 0;

    virtual void StdError() const;

  protected:
    U32 What() const;     // what (code form of 'name')
    U32 _what;            // storage for what

    const char* _who;     // name of function throwing exception
    const char* _where;   // source:line info
    const char* _reason;  // optional, provides context-specific reason

    char _wherebuffer[CX_EXCEPTIONSIZE];
    void SetWhere(const char* where);

    char _reasonbuffer[CX_EXCEPTIONSIZE];
    void SetReason(const char* format, va_list pArgs);
  };



  // this design pattern is called "value-specialized template"
  // it is used to clone classes exactly, via a template
  template<typename Tag,
           typename TWhat,
           std::map<TWhat, const char*>& TWhatNameMap,
           std::map<TWhat, const char*>& TWhatStringMap,
           typename TBase=BaseException>
  class ExceptionTemplate: public TBase
  {
  protected:
    ExceptionTemplate(const char* who, const char* where,
                      const U32 what)
                      : TBase(who, where, what)
    {
      // this protected constructor exists so that every derived
      // base class supports a 'what' parameter that is U32, just
      // like CXBaseException.
    }


  public:
    ExceptionTemplate(const char* who, const char* where,
                      TWhat what=TWhat::NONE)
                      : TBase(who, where, static_cast<U32>(what))
    {
      TBase::_reason = nullptr;
    }

    ExceptionTemplate(const char* who, const char* where,
                      TWhat what=TWhat::NONE,
                      const char* reason=nullptr, ...)
                      : TBase(who, where, static_cast<U32>(what))
    {
      va_list pArgs;
      va_start(pArgs, reason);
      TBase::SetReason(reason, pArgs);
      va_end(pArgs);
    }

    TWhat What() const
    {
      // the 'static_cast' is needed because our storage for
      // the what code ('TBase::_what') is a U32 there
      return static_cast<TWhat>(TBase::_what);
    }

    // 'why' is a generic, canned version of 'reason'
    // it is indexed via the 'what' code.
    virtual const char* Why() const
    {
      return TWhatStringMap[What()];
    }

    virtual const char* Name() const
    {
      return TWhatNameMap[What()];
    }
  };


  #define CX_EXCEPTIONS                                               \
    X(NONE,  "Something ineffable happened.")

  #define X(v,str) v,
  enum class Error: U32
  {
    CX_EXCEPTIONS
  };
  extern std::map<Error, const char*> ErrorNameMap;
  extern std::map<Error, const char*> ErrorStringMap;
  #undef X

  // this is used to declare exception types that derive
  // from CXBaseException
  #define CX_DECLARE_BASE_EXCEPTION_CLASS(Type, TEnum)                \
    typedef                                                           \
    CX::ExceptionTemplate<struct Type##Tag, TEnum, TEnum##NameMap,    \
    TEnum##StringMap> Type;

  // CX::Exception is used for reporting error situations
  // that client code should be able to handle non-fatally.
  CX_DECLARE_BASE_EXCEPTION_CLASS(Exception, Error);

  // this is used to declare exception types that derive from something
  // other than CXBaseException (directly).
  // See CX::Args::Exception for example.
  #define CX_DECLARE_EXCEPTION_CLASS(Type, TEnum, Base)               \
    typedef                                                           \
    CX::ExceptionTemplate<struct Type##Tag, TEnum, TEnum##NameMap,    \
    TEnum##StringMap, Base> Type;

} // namespace 'CX'

#define CX_THROW(object, why, ...)                                    \
{                                                                     \
  CX_DEBUGOUT(CX::thrown_message, cx_trace_methodname);               \
  CX::flush();                                                        \
  throw object(cx_trace_methodname,                                   \
      __FILE__ ":" CX_STRINGIZE(__LINE__), why, __VA_ARGS__);         \
}

#ifdef CX_OPT_TRACING
  #define CX_TRACE_RESET                                              \
    CX::set_tracelevel(cx_catchlevel);                                \
    CX_TRACEOUT(CX::trace_reset_message)

  #define CX_TRY                                                      \
    CX_DIV0ASSERT(cx_traceflag); /* enforces use of CX_METHOD, etc */ \
    cx_catchlevel=CX::get_tracelevel();                               \
    try {

  #define CX_CATCH(type)                                              \
    }                                                                 \
    catch (type)                                                      \
    {                                                                 \
      CX_DIV0ASSERT(cx_catchlevel);                                   \
      CX_TRACE_RESET;                                                 \
      CX_DEBUGOUT(CX::caught_message, cx_trace_methodname);           \
      CX::flush();

  #define CX_ENDTRY }
#else
  #define CX_TRY try {
  #define CX_CATCH(type) } catch (type) {
  #define CX_ENDTRY }
#endif


#endif  // CX_EXCEPTIONS_HPP

