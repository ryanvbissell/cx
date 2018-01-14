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

CX::OS::PathList g_plist;


void init_g_plist()
{
  for (auto const& pathstr : TestPaths)
  {
    CX::OS::Path p = CX::OS::Path(pathstr);
    g_plist += p;
  }
}


void test_pathlist_delim()
{
  // TODO, this is OS-specific
  CX_TEST_ASSERT(':' == CX::OS::PathList::Delim());
  CX_TEST_ASSERT(CX::OS::PathList::IsDelim(':'));
}


void test_pathlist_concat()
{
  std::vector<CX::OS::Path> list_of_paths;

  for (auto const p : g_plist)
    list_of_paths.push_back(p);

  printf("%s\n\n", g_plist.str().c_str());

  std::vector<std::string>::iterator sitr = TestPaths.begin();
  auto pitr = list_of_paths.begin();
  for (auto const p : g_plist)
  {
    std::string original(*sitr);
    std::string pathified(pitr->str());
    std::string list_extracted(p.str());
    printf("%15s\t\t:\t%15s  -->  %-15s\t%s\n",
        str_or_empty(original).c_str(),
        str_or_empty(pathified).c_str(),
        str_or_empty(list_extracted).c_str(),
        list_extracted == pathified ? "PASS" : "FAIL" );
    ++sitr;
    ++pitr;
  }
  printf("\n");
}


void test_pathlist_init_from_string()
{
  CX::OS::PathList second(g_plist.str());

  printf("%s\n\n", second.str().c_str());
  CX_TEST_ASSERT(second.str() == g_plist.str());
}


int main(int argc, char** argv)
{
  init_g_plist();

  test_pathlist_delim();

  test_pathlist_concat();
  test_pathlist_init_from_string();
}



