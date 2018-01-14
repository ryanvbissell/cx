// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2013-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef LINUX_PROCESS_HPP
#define LINUX_PROCESS_HPP


class Process_Linux : public ProcessBase<Process_Linux>
{
  //friend ProcessBase<Process_Linux>;

  public:
    Process_Linux(CXPID pid);

  protected:
    Path _execSpec() const;
    void _registerSet(size_t* size, void* buf) const;
};

using Process = Process_Linux;
template<> Path ProcessBase<Process>::_execSpec() const;
template<> void ProcessBase<Process>::_registerSet(size_t* size, void* buf) const;


#endif // LINUX_PROCESS_HPP
