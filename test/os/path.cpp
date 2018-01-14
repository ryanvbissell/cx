// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TESTING

#include "cx-test-support.hpp"
#include "cx-os.hpp"

#include "testpaths.hpp"

using std::string;
using CX::OS::Path;

std::vector<string> reasons;

void iterate_over_elements(string const& p, Path const& path)
{
  printf("%-15s\t\t", (str_or_empty(p) + ":").c_str());
  Path::const_iterator it = path.cbegin();
  if (it == path.cend())
    printf("<empty>\n");
  else
    for(Path::const_iterator it = path.cbegin(); it != path.cend(); ++it)
      printf("%s%s", it->c_str(), it+1 == path.cend() ? "\n" : ",");
}


void iterate_over_elements()
{
  printf("%s\n", "ITERATE OVER ELEMENTS");
  for (string p : TestPaths)
  {
    Path path(p);
    iterate_over_elements(p, path);
  }
  printf("\n\n");
}



void string_representation(string const& p, Path const& path)
{
  printf("%-15s\t\t", (str_or_empty(p) + ":").c_str());
  printf("%s\n", str_or_empty(path.str()).c_str());
}


void string_representation()
{
  printf("%s\n", "STRING REPRESENTATION");
  for (string p : TestPaths)
  {
    Path path(p);
    string_representation(p, path);
  }
  printf("\n\n");
}



void normalized_representation(string const& p, Path const& path)
{
  printf("%-15s\t\t", (str_or_empty(p) + ":").c_str());
  printf("%s\n", str_or_empty(path.LexicallyNormal().str()).c_str());
}


void normalized_representation()
{
  printf("%s\n", "LEXICALLY-NORMAL REPRESENTATION");
  for (string p : TestPaths)
  {
    Path path(p);
    normalized_representation(p, path);
  }
  printf("\n\n");
}


void test_os_validity(string const& p, Path const& path)
{
  printf("%-15s\t\t", (str_or_empty(p) + ":").c_str());
  printf("%s\n", path.IsValid() ? "YES" : "NO");
}


void test_os_validity()
{
  printf("%s\n", "TESTING OS VALIDITY");
  string winp("C:/foo/bar.txt");
  Path winpath(winp);
  test_os_validity(winp, winpath);
  for (string p : TestPaths)
  {
    Path path(p);
    test_os_validity(p, path);
  }
  printf("\n\n");
}


void test_absolute_or_relative(string const& p, Path const& path)
{
  printf("%-15s\t\t", (str_or_empty(p) + ":").c_str());
  printf("%s\n", path.IsAbsolute() ? "absolute"
                                   : path.IsRelative() ? "relative"
                                                       : "<NEITHER>");
}


void test_absolute_or_relative()
{
  printf("%s\n", "TESTING ABSOLUTISM");
  for (string p : TestPaths)
  {
    Path path(p);
    test_absolute_or_relative(p, path);
  }
  printf("\n\n");
}


void test_lexical_relativity(string const& p, string const& b)
{
  Path path(p);
  Path base(b);

  printf("%-19s %-18s %s\n",
      str_or_empty(p).c_str(),
      str_or_empty(b).c_str(),
      str_or_empty(path.LexicallyRelative(base).str()).c_str());
}


void test_lexical_relativity()
{
  printf("%s\n", "TESTING LEXICAL-RELATIVITY");
  printf("%-19s %-18s %s\n", "PATH", "BASE", "RELATIVE");
  test_lexical_relativity(Path::dot(), "/foo/bar");
  test_lexical_relativity(Path::dotdot(), "/foo/bar");
  test_lexical_relativity("", "/foo/bar");
  test_lexical_relativity("/foo/bar/quux", "");
  test_lexical_relativity("/foo/bar/quux", "/");
  test_lexical_relativity("/foo/bar/quux", "/foo");
  test_lexical_relativity("/foo/bar/quux", "/foo/bar");
  test_lexical_relativity("/foo/bar/quux", "/foo/bar/quux");
}


void test_lexical_absolutism(string const& p, string const& b)
{
  Path path(p);
  Path base(b);
  string abs;

  CX_TRY
  {
    Path abspath = path.LexicallyAbsolute(base);
    abs = str_or_empty(abspath.str());
  }
  CX_CATCH(CX::OS::Exception const& e)
  {
    reasons.push_back(e.Reason());
    abs = "<throws, reason #" + std::to_string(reasons.size()) + ">";
  }
  CX_ENDTRY

  printf("%-19s %-18s %s\n",
      str_or_empty(p).c_str(),
      str_or_empty(b).c_str(),
      abs.c_str());
}


void test_lexical_absolutism()
{
  reasons.clear();
  printf("\n\n%s\n", "TESTING LEXICAL-ABSOLUTISM");
  printf("%-19s %-18s %s\n", "PATH", "BASE", "ABSOLUTE");

  // these are expected to work
  test_lexical_absolutism("", "/foo/bar/quux");
  test_lexical_absolutism("quux", "/foo/bar");
  test_lexical_absolutism("bar/quux", "/foo");
  test_lexical_absolutism("foo/bar/quux", "/");
  test_lexical_absolutism("/bar/quux", "/foo");
  test_lexical_absolutism("quux", "//net/foo/bar");

  // these are expected to throw
  test_lexical_absolutism("bar/quux", "foo");
  test_lexical_absolutism("//quux", "//net/foo/bar");

  if (reasons.size())
  {
    printf("\n");
    for (size_t r=0; r<reasons.size(); ++r)
      printf("[%lu] '%s'\n", r+1, reasons[r].c_str());
  }
}



void test_path_components(string const& p, Path const& path)
{
  printf("%-16s\t\t", (str_or_empty(p) + ":").c_str());
  printf("%-16s%-16s%-16s%-16s%-16s%-16s%-16s%-16s\n",
          str_or_empty(path.RootPath().str()).c_str(),
          str_or_empty(path.RootName().str()).c_str(),
          str_or_empty(path.RootDirectory().str()).c_str(),
          str_or_empty(path.RelativePath().str()).c_str(),
          str_or_empty(path.ParentPath().str()).c_str(),
          str_or_empty(path.Filename().str()).c_str(),
          str_or_empty(path.Stem()).c_str(),
          str_or_empty(path.Extension()).c_str() );
}


void test_path_components()
{
  printf("\n\n%s\n", "TESTING PATH COMPONENTS");
  printf( "%-16s\t\t%-16s%-16s%-16s%-16s%-16s%-16s%-16s%-16s\n",
          " ",
          "RootPath()",
          "RootName()",
          "RootDirectory()",
          "RelativePath()",
          "ParentPath()",
          "Filename()",
          "Stem()",
          "Extension()");
  for (string p : TestPaths)
  {
    Path path(p);
    test_path_components(p, path);
  }
  printf("\n\n");
}


int main(int argc, char** argv)
{
  iterate_over_elements();
  string_representation();
  normalized_representation();
  test_os_validity();
  test_absolute_or_relative();
  test_lexical_relativity();
  test_lexical_absolutism();

  // I decree that this should always be the last test
  test_path_components();
}



