// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef LINUX_PATH_HPP
#define LINUX_PATH_HPP


class Path_Linux : public PathBase<Path_Linux>
{
  public:
    Path_Linux();
    Path_Linux(std::string const& p);
    Path_Linux(PathBase<Path_Linux> const&);

  private:
    bool _isValid() const;
    std::string _separators() const;
    void _initRootName(std::string* text);
};

using Path = Path_Linux;
template<> bool PathBase<Path>::_isValid() const;
template<> std::string PathBase<Path>::_separators() const;
template<> void PathBase<Path>::_initRootName(std::string* text);


#endif // LINUX_PATH_HPP
