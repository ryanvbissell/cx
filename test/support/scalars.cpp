// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :

/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#include "cx-test-support.hpp"
#include "cx-types.hpp"

#include <stdlib.h>

int main(int argc, char** argv)
{
  printf("Verifying expected sizes of scalar types...\n");
  CX_TEST_ASSERT(sizeof(S8) == 1);
  CX_TEST_ASSERT(sizeof(U8) == 1);
  CX_TEST_ASSERT(sizeof(S16) == 2);
  CX_TEST_ASSERT(sizeof(U16) == 2);
  CX_TEST_ASSERT(sizeof(S32) == 4);
  CX_TEST_ASSERT(sizeof(U32) == 4);
  CX_TEST_ASSERT(sizeof(S64) == 8);
  CX_TEST_ASSERT(sizeof(U64) == 8);

  exit(EXIT_SUCCESS);
}

