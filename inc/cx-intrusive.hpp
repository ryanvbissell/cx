// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef CX_INTRUSIVE_HPP
#define CX_INTRUSIVE_HPP

#include <type_traits>
#include <typeinfo>
#include <atomic>

#include "cx-tracedebug.hpp"

namespace CX
{

  class IntrusiveBase
  {
  public:
    IntrusiveBase();
    virtual ~IntrusiveBase();

    bool Nascent() const { return nascent_; }
    size_t Refcount() const { return refcount_; }
    virtual void Adopted() const;
    virtual bool Disowned() const;

  protected:
    mutable bool nascent_;
    mutable std::atomic<size_t> refcount_;
  };


  template <class T>
  class IntrusivePtr
  {
  template <class U> friend class IntrusivePtr;  // for non-covariant 'thys_' access

  public:
    using IPtr = IntrusivePtr<T>;

    IntrusivePtr(); // initializes to nullptr
    IntrusivePtr(std::nullptr_t);
    IntrusivePtr(T* that, bool addRef=true);
    IntrusivePtr(T const* that, bool addRef=true);
    IntrusivePtr(IPtr const& that);
    IntrusivePtr(IPtr&& that);
#if 1 /* c++14 */
    // implicit upcasting
    template <class U,
              class = std::enable_if_t<std::is_convertible<U*, T*>::value> >
    IntrusivePtr(IntrusivePtr<U> const& that) :thys_(that.thys_)
    {
      CX_TOPICOUT(cx:iptr,
                  "[[covariant pseudo-upcast of IPtr<%s> to IPtr<%s>]]\n",
                  get_typename<U>().c_str(), get_typename<T>().c_str());
      adopt();
    }

    // explicit downcasting
    template <class U,
              class = std::enable_if_t<std::is_convertible<T*, U*>::value>>
    explicit IntrusivePtr(IntrusivePtr<U> const& that, int=0) :thys_(static_cast<T*>(that.thys_))
    {
      CX_TOPICOUT(cx:iptr,
                  "[[covariant pseudo-downcast of IPtr<%s> to IPtr<%s>]]\n",
                  get_typename<U>().c_str(), get_typename<T>().c_str());
      // TODO, make sure this version doesn't have the same problem as
      //       the c++11 version (note dynamic_cast in the latter)
      adopt();
    }
#else /* c++11 */
    // implicit upcasting
    template <class U,
              class = class std::enable_if<std::is_convertible<U*, T*>::value>::type>
    IntrusivePtr(IntrusivePtr<U> const& that) :thys_(that.thys_)
    {
      CX_TOPICOUT(cx:iptr,
                  "[[covariant pseudo-upcast of IPtr<%s> to IPtr<%s>]]\n",
                  get_typename<U>().c_str(), get_typename<T>().c_str());
      adopt();
    }

    // explicit downcasting
    template <class U,
              class = class std::enable_if<std::is_convertible<T*, U*>::value>::type>
    explicit IntrusivePtr(IntrusivePtr<U> const& that, int=0) :thys_(dynamic_cast<T*>(that.thys_))
    {
      CX_TOPICOUT(cx:iptr,
                  "[[covariant pseudo-downcast of IPtr<%s> to IPtr<%s>]]\n",
                  get_typename<U>().c_str(), get_typename<T>().c_str());
      // Curiously, std::is_convertible doesn't keep this constructor from
      // being called when U is not a sub-class of T.
      // However, the dynamic_cast will return nullptr in that case
      CX_ASSERT(thys_ != nullptr, "Inappropriate downcast");
      adopt();
    }
#endif
    virtual ~IntrusivePtr();

    IPtr& operator=(IPtr const& that);
    IPtr& operator=(IPtr&& that);

    T& operator*() const;
    T* operator->() const;
    explicit operator bool() const;

    T* get() const { return thys_; }

  private:
    T* thys_; // is required to have 'mutable long refcount_;'

    void adopt();
    void disown();

    class ThouShallNotDelete;
    operator ThouShallNotDelete*() const;
  };


  template <class L, class R>
  CX_FUNCTION(bool operator==,  IntrusivePtr<L> const& lhs,
                                IntrusivePtr<R> const& rhs)

    CX_RETURN(lhs.get() == rhs.get());

  CX_ENDFUNCTION


  template <class L, class R>
  CX_FUNCTION(bool operator!=,  IntrusivePtr<L> const& lhs,
                                IntrusivePtr<R> const& rhs)

    CX_RETURN(lhs.get() != rhs.get());

  CX_ENDFUNCTION


  template <class T>
  CX_FUNCTION(bool operator==,  IntrusivePtr<T> const& lhs, std::nullptr_t)

    CX_RETURN(lhs.get() == nullptr);

  CX_ENDFUNCTION


  template <class T>
  CX_FUNCTION(bool operator!=,  IntrusivePtr<T> const& lhs, std::nullptr_t)

    CX_RETURN(lhs.get() != nullptr);

  CX_ENDFUNCTION


  template <class T>
  CX_FUNCTION(bool operator==,  std::nullptr_t, IntrusivePtr<T> const& rhs)

    CX_RETURN(nullptr == rhs.get());

  CX_ENDFUNCTION


  template <class T>
  CX_FUNCTION(bool operator!=,  std::nullptr_t, IntrusivePtr<T> const& rhs)

    CX_RETURN(nullptr != rhs.get());

  CX_ENDFUNCTION
}

#include "../src/cx-intrusive.incl"

#endif  // CX_INTRUSIVE_HPP

