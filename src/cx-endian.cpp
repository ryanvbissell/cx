// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2013-2017 Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "endian"

#include "cx-endian.hpp"


using namespace CX::Endian;

template<>
U16 detail::bswap<U16>(U16 val) { return CX_BSWAP16(val); }

template<>
S16 detail::bswap<S16>(S16 val) { return CX_BSWAP16((U16)val); }

template<>
U32 detail::bswap<U32>(U32 val) { return CX_BSWAP32(val); }

template<>
S32 detail::bswap<S32>(S32 val) { return CX_BSWAP32((U32)val); }

template<>
U64 detail::bswap<U64>(U64 val) { return CX_BSWAP64(val); }

template<>
S64 detail::bswap<S64>(S64 val) { return CX_BSWAP64((U64)val); }

