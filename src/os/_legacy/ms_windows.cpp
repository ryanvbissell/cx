// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "os"

#include "os/ms_windows.hpp"
#include "cx-exceptions.hpp"

#include <windows.h>

using CX::OS::PID;

using std::string;


CX_CONSTRUCTOR(CX::OS::Path__ms_windows::Path__ms_windows,
                                                  :CX::OS::PathBase())
CX_ENDMETHOD


CX_CONSTMETHOD(const char* CX::OS::Path__ms_windows::_delims)

  CX_RETURN("\\");

CX_ENDMETHOD



CX_CONSTMETHOD(bool CX::OS::Path__ms_windows::_exists, string const &path)

  DWORD dwAttrib;

  dwAttrib = GetFileAttributes(path.c_str());

  if (dwAttrib == INVALID_FILE_ATTRIBUTES)
    CX_RETURN(false);

  // whatever it is (file, fifo, symlink, etc.) it apparently exists.
  CX_RETURN(true);

CX_ENDMETHOD



CX_CONSTMETHOD(bool CX::OS::Path__ms_windows::_isFile, string const &path)

  DWORD dwAttrib;

  dwAttrib = GetFileAttributes(path.c_str());

  if (dwAttrib == INVALID_FILE_ATTRIBUTES)
    CX_RETURN(false);

  if (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)
    CX_RETURN(false);

  // I think anything else is a file?
  CX_RETURN(true);

CX_ENDMETHOD



CX_CONSTMETHOD(bool CX::OS::Path__ms_windows::_isSymlink,
                                                    string const &path)
  CX_ASSERT(false); // TODO

CX_ENDMETHOD



CX_CONSTMETHOD(std::string CX::OS::Path__ms_windows::_readlink,
                                                    string const &path)
  CX_ASSERT(false); // TODO

CX_ENDMETHOD


// ====================================================================


CX_CONSTRUCTOR(CX::OS::File__ms_windows::File__ms_windows,
                                                    :CX::OS::FileBase())
CX_ENDMETHOD



CX_DESTRUCTOR(CX::OS::File__ms_windows::~File__ms_windows)
CX_ENDMETHOD



CX_CONSTMETHOD(U8* CX::OS::File__ms_windows::_openMapped,
                                                string const &name,
                                                int *fd, size_t *psize)
  CX_ASSERT(false); // TODO

CX_ENDMETHOD



CX_CONSTMETHOD(void CX::OS::File__ms_windows::_unmapAndClose, int fd,
                                                            U8* buffer,
                                                            size_t size)
  CX_ASSERT(false); // TODO

CX_ENDMETHOD


// ====================================================================


CX_CONSTRUCTOR(CX::OS::Process__ms_windows::Process__ms_windows,
                                                :CX::OS::ProcessBase())
CX_ENDMETHOD



CX_DESTRUCTOR(CX::OS::Process__ms_windows::~Process__ms_windows)
CX_ENDMETHOD




CX_CONSTMETHOD(PID CX::OS::Process__ms_windows::_spawn,
                                                string const &execspec,
                                                ArgsVector const &args,
                                                bool traced)
  CX_ASSERT(false); // TODO

CX_ENDMETHOD



CX_CONSTMETHOD(string CX::OS::Process__ms_windows::_getExecSpec, PID pid)

  CX_ASSERT(false); // TODO

CX_ENDMETHOD



CX_CONSTMETHOD(void CX::OS::Process__ms_windows::_getRegSet, PID pid,
                                                           size_t* size,
                                                           void* buf)
  CX_ASSERT(false); // TODO

CX_ENDMETHOD


