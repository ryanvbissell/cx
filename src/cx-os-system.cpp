// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "os"

#include "cx-os.hpp"
#include "cx-exceptions.hpp"

using std::string;

namespace CX {
namespace OS {


extern System g_system;


template <class T>
CX_CONSTRUCTOR0(SystemBase<T>::SystemBase)

CX_ENDMETHOD



template <class T>
CX_METHOD(string SystemBase<T>::GetHardwarePlatform)

  CX_RETURN(g_system._getHardwarePlatform());

CX_ENDMETHOD


// ===================================================================


template class SystemBase<System>;

// the one and only instantiation of CX::OS::System
System g_system;

} // namespace 'OS'
} // namespace 'CX'
