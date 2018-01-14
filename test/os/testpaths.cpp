// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#include <vector>
#include <string>

std::vector<std::string> TestPaths {
  "",
  ".",
  "..",
  "foo",
  "/",
  "/foo",
  "foo/",
  "/foo/",
  "foo/bar",
  "/foo/bar",
  "//net",
  "//net/foo",
  "///foo///",
  "///foo///bar",
  "/.",
  "./",
  "/..",
  "../",
  "foo/.",  // the ParentPath() of this one will render different than boost, on purpose
  "foo/..",   // this one is redundant, but exists in the boost "Path decomposition table"
  "foo/./",
  "foo/./bar",
  "foo/..",
  "foo/../",
  "foo/../bar",
  ".foo/",
  "empty.",
  "stem.txt",
  ".foorc"
};


std::string str_or_empty(std::string const& s)
{
  return s.empty() ? "<empty>" : s;
}


char const* cstr_or_null_or_empty(char const* p)
{
  return p ? (*p ? p : "<empty>") : "<null>";
}

