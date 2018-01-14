// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef LINUX_PATHLIST_HPP
#define LINUX_PATHLIST_HPP


class PathList_Linux : public PathListBase<PathList_Linux>
{
  public:
    PathList_Linux();
    PathList_Linux(std::string const& p);
    PathList_Linux(PathListBase<PathList_Linux> const&);

  private:
    static char _delim();
};

using PathList = PathList_Linux;

template<> char PathListBase<PathList>::_delim();


#endif // LINUX_PATHLIST_HPP
