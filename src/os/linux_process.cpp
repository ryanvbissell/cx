// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2013-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "os"

#include <sys/ptrace.h>
//#include <sys/wait.h>
//#include <unistd.h>
#include <fcntl.h>     // for iovec
#include <linux/elf.h> // for NT_PRSTATUS and friends

#include <sstream>

#include "cx-os.hpp"
#include "cx-exceptions.hpp"

using std::string;

namespace CX {
namespace OS {


CX_CONSTRUCTOR(Process_Linux::Process_Linux,
                                      : ProcessBase<Process_Linux>(pid),
                                                              CXPID pid)
CX_ENDMETHOD



template <>
CX_CONSTMETHOD(Path ProcessBase<Process_Linux>::_execSpec)

  Path result;
  std::stringstream proc_pid_exe;

  proc_pid_exe << "/proc/" << (pid_t)_pid << "/exe";

  result = Shell::ResolveSymlink(proc_pid_exe.str());
  CX_RETURN(result);

CX_ENDMETHOD



template <>
CX_CONSTMETHOD(void ProcessBase<Process_Linux>::_registerSet,  size_t* size,
                                                          void* buf)

  long ret;
  iovec regvec;

  regvec.iov_len = *size;
  regvec.iov_base = buf;
  ret = ::ptrace(PTRACE_GETREGSET, (pid_t)_pid, NT_PRSTATUS, &regvec);
  if (ret < 0)
  {
    CX_ASSERT(false);  // TODO, need exception here
  }
  if (regvec.iov_len != *size)
  {
    CX_ASSERT(false); // TODO, warning or exception?
  }
  *size = regvec.iov_len;

CX_ENDMETHOD


} // namespace OS
} // namespace CX

