// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2013-2017 Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef CX_ENDIAN_HPP
#define CX_ENDIAN_HPP

#include "cx-hackery.hpp"

namespace CX {
namespace Endian {

  // XXX: we don't use the standard 'ntohX' routines, because...
  // 1. MinGW32's header files don't provide ntohll()
  // 2. The various OSes we support cannot agree on which header
  //    file they belong in.
  // So, we roll our own instead.
  //////////#include <netinet/in.h>   /* some *nix */
  //////////#include <arpa/inet.h>    /* some *nix */
  //////////#include <winsock2.h>     /* windows */

  namespace detail
  {
    template <typename T> T bswap(T value); // explicit specializations

    template<> U16 bswap<U16>(U16 value);
    template<> S16 bswap<S16>(S16 value);
    template<> U32 bswap<U32>(U32 value);
    template<> S32 bswap<S32>(S32 value);
    template<> U64 bswap<U64>(U64 value);
    template<> S64 bswap<S64>(S64 value);
  };

  template <typename T> T betoh(T value)
  {
    if (CX_HOST_IS_BIG_ENDIAN)
      return value;

      return detail::bswap(value);
  }

  template <typename T> T letoh(T value)
  {
    if (CX_HOST_IS_LITTLE_ENDIAN)
      return value;

    return detail::bswap(value);
  }


  template <typename T> T htobe(T value)
  {
    return betoh(value);
  }


  template <typename T> T htole(T value)
  {
    return letoh(value);
  }

} // namespace 'Endian'
} // namespace 'CX'


#endif // CX_ENDIAN_HPP

