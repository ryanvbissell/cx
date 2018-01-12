// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef CX_HACKERY_HPP
#define CX_HACKERY_HPP

#include "cx-types.hpp"

#define CX_HOST_IS_BIG_ENDIAN (*(U16 *)"\0\xff" < 0x100)
#define CX_HOST_IS_LITTLE_ENDIAN (!CX_HOST_IS_BIG_ENDIAN)


#define CX_MIN(a,b) ( (a) < (b) ? (a) : (b) )
#define CX_MAX(a,b) ( (a) > (b) ? (a) : (b) )

#define BIT(n) ( UINT64_C(1) << (n) )

#define CX_POW2(n) ((n) == ((n) & (~(n) + 1)))

#define CX_ONEBITSET(n) ((n) && CX_POW2(n))

#define CX_BSWAP16(val)                         \
      [](U16 u16)                               \
      {                                         \
        return  ( (((u16) << 8*1) & 0xFF00) |   \
                  (((u16) >> 8*1) & 0x00FF) );  \
      }((val))

#define CX_BSWAP32(val)                               \
      [](U32 u32)                                     \
      {                                               \
        return  ( (((u32) << 8*3) & 0xFF000000UL) |   \
                  (((u32) << 8*1) & 0x00FF0000UL) |   \
                  (((u32) >> 8*1) & 0x0000FF00UL) |   \
                  (((u32) >> 8*3) & 0x000000FFUL) );  \
      }((val))

#define CX_BSWAP64(val)                                       \
      [](U64 u64)                                             \
      {                                                       \
        return  ( (((u64) << 8*7) & 0xFF00000000000000ULL) |  \
                  (((u64) << 8*5) & 0x00FF000000000000ULL) |  \
                  (((u64) << 8*3) & 0x0000FF0000000000ULL) |  \
                  (((u64) << 8*1) & 0x000000FF00000000ULL) |  \
                  (((u64) >> 8*1) & 0x00000000FF000000ULL) |  \
                  (((u64) >> 8*3) & 0x0000000000FF0000ULL) |  \
                  (((u64) >> 8*5) & 0x000000000000FF00ULL) |  \
                  (((u64) >> 8*7) & 0x00000000000000FFULL) ); \
      }((val))


#define CX_STRCAT(s1, s2) (std::string(s1) + std::string(s2))

#define CX_STRINGIZE2(n) #n
#define CX_STRINGIZE(n) CX_STRINGIZE2(n)

#ifdef __GNUC__
#define CX_LIKELY(x)   __builtin_expect(!!(x), 1)
#define CX_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define CX_LIKELY(x)   (x)
#define CX_UNLIKELY(x) (x)
#endif

#define CX_FINAL final

#endif // CX_HACKERY_HPP
