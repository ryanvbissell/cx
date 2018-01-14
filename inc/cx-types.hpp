// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2013-2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef CX_TYPES_HPP
#define CX_TYPES_HPP

// this file should be modified as exceptions are encountered.
// it begins life optimistic that the ideal applies everywhere.

// it also assumes C++11

#include <climits>
#include <cstdint>
#include <cinttypes>

typedef int8_t   S8;
typedef int16_t  S16;
typedef int32_t  S32;
typedef int64_t  S64;

typedef uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef U64 CXPID;

#endif // CX_TYPES_HPP


