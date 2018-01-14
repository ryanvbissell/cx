// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef LINUX_SYSTEM_HPP
#define LINUX_SYSTEM_HPP


class System_Linux : public SystemBase<System_Linux>
{
  friend SystemBase<System_Linux>;

  public:
    System_Linux();

  protected:
    std::string _getHardwarePlatform() const;
};

using System = System_Linux;
template<> std::string SystemBase<System>::_getHardwarePlatform() const;


#endif // LINUX_SYSTEM_HPP
