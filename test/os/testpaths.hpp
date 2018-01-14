// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef TESTPATHS_HPP
#define TESTPATHS_HPP

extern std::vector<std::string> TestPaths;

std::string str_or_empty(std::string const& s);
char const* cstr_or_null_or_empty(char const* p);

#endif

