// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef CX_OS_ANDROID_HPP
#define CX_OS_ANDROID_HPP

#include "os/gnu_linux.hpp"

namespace CX {
namespace OS {

  class Path__android: public CX::OS::Path__gnu_linux
  {
  public:
    Path__android();
    virtual ~Path__android();
  };


  class File__android: public CX::OS::File__gnu_linux
  {
  public:
    File__android();
    virtual ~File__android();
  };


  class Process__android: public CX::OS::Process__gnu_linux
  {
  public:
    Process__android();
    virtual ~Process__android();
  };

} // namespace 'OS'
} // namespace 'CX'

#endif  // CX_OS_ANDROID_HPP
