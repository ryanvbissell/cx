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


void Test_BIT()
{
  U64 bit, value;

  printf("Testing the BIT() macro...\n");

  for (value=1, bit=0; value; ++bit, value <<= 1)
    CX_TEST_ASSERT(value == BIT(bit));
}


void Test_ONEBITSET()
{
  printf("Testing CX_ONEBITSET()...\n");

  CX_TEST_ASSERT(!CX_ONEBITSET(0));
  CX_TEST_ASSERT( CX_ONEBITSET(1));
  CX_TEST_ASSERT( CX_ONEBITSET(2));
  CX_TEST_ASSERT(!CX_ONEBITSET(3));
  CX_TEST_ASSERT( CX_ONEBITSET(4));
  CX_TEST_ASSERT(!CX_ONEBITSET(5));
}


int main(int argc, char** argv)
{
  Test_BIT();
  Test_ONEBITSET();

  exit(EXIT_SUCCESS);
}

