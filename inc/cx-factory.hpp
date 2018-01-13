// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2013-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef CX_FACTORY_HPP
#define CX_FACTORY_HPP

#include <unordered_map>

namespace CX
{

  template<typename TBase, typename TClass, typename... TCtorParams>
  TBase* CreateObject(TCtorParams... params)
  {
    return new TClass(params...);
  }

  template<typename TBase, typename TKey, typename... TCtorParams>
  class Factory
  {
  protected:
    typedef TBase *(*FactoryFunc)(TCtorParams... params);

  public:
    typedef typename std::unordered_map<TKey, FactoryFunc> FFuncMap;
    typedef typename FFuncMap::const_iterator const_iterator;
    typedef typename FFuncMap::iterator iterator;

    template<typename TClass>
    bool Define(TKey key)
    {
      if (ffuncs_.find(key) != end())
        return false;

      ffuncs_[key] = &CreateObject<TBase, TClass, TCtorParams...>;
      return true;
    }

    TBase* New(TKey key, TCtorParams... params)
    {
      const_iterator iter = ffuncs_.find(key);
      if (iter == cend())
        return nullptr;

      return (iter->second)(params...);
    }

    const_iterator cbegin() const
    {
      return ffuncs_.cbegin();
    }

    const_iterator cend() const
    {
      return ffuncs_.cend();
    }

    iterator begin()
    {
      return ffuncs_.begin();
    }

    iterator end()
    {
      return ffuncs_.end();
    }

    size_t size() const
    {
      return ffuncs_.size();
    }

  private:
    FFuncMap ffuncs_;
  };

}

#endif  // CX_FACTORY_HPP
