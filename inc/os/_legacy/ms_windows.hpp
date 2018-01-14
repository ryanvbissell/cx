// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef CX_OS_MS_WINDOWS_HPP
#define CX_OS_MS_WINDOWS_HPP

#include "cx-os.hpp"

namespace CX {
namespace OS {

  class Path__ms_windows: public PathBase
  {
  public:
    Path__ms_windows();
    virtual ~Path__ms_windows();

  private:
    virtual char const * _delims() const;
    virtual bool _exists(std::string const &path) const;
    virtual bool _isFile(std::string const &path) const;
    virtual bool _isSymlink(std::string const &path) const;
    virtual std::string _readlink(std::string const &path) const;
  };


  class File__ms_windows: public FileBase
  {
  public:
    File__ms_windows();
    virtual ~File__ms_windows();

  private:
    virtual U8 * _openMapped(std::string const &name, int *fd, size_t *psize) const;
    virtual void _unmapAndClose(int fd, U8 *pbuf, size_t size) const;
  };


  class Process__ms_windows: public ProcessBase
  {
  public:
    Process__ms_windows();
    virtual ~Process__ms_windows();

  private:
    virtual PID _spawn(std::string const &execspec, ArgsVector const &args, bool traced) const;
    virtual std::string _getExecSpec(PID pid) const;
    virtual void _getRegSet(PID pid, size_t *psize, void *pbuf) const;
  };


} // namespace 'OS'
} // namespace 'CX'


#endif  // CX_OS_MS_WINDOWS_HPP
