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

#include <assert.h>
#include <utility>

using std::string;

namespace CX {
namespace OS {


template <class T>
CX_CONSTRUCTOR(ProcessBase<T>::ProcessBase, :_pid(pid), CXPID pid)

CX_ENDMETHOD


template <class T>
CX_METHOD(CXPID ProcessBase<T>::GetPid)

  CX_RETURN(_pid);

CX_ENDMETHOD


template <class T>
CX_METHOD(Path ProcessBase<T>::ExecSpec)

  CX_RETURN(_execSpec());

CX_ENDMETHOD



template <class T>
CX_METHOD(void ProcessBase<T>::RegisterSet, size_t* size, void* buf)

  CX_RETURN(_registerSet(size, buf));

CX_ENDMETHOD



template class ProcessBase<Process>;


} // namespace 'OS'
} // namespace 'CX'
