// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :

/*
 * Copyright (c) 2016-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#include "cx-test-support.hpp"
#include "cx-hackery.hpp"

void Test_POW2()
{
  printf("Testing CX_POW2()...\n");

  CX_TEST_ASSERT( CX_POW2(0) );
  CX_TEST_ASSERT(!CX_POW2(3) );

  for (U64 value=1; value; value <<= 1)
    CX_TEST_ASSERT(CX_POW2(value));
}


int main(int argc, char** argv)
{
  Test_POW2();

  exit(EXIT_SUCCESS);
}

