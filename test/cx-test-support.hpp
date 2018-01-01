// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef CX_TEST_SUPPORT
#define CX_TEST_SUPPORT

#include <stdio.h>
#include <stdlib.h>

#define CX_TEST_ASSERT(expr) \
        cx_test_assert(expr, #expr, __FILE__, __LINE__);

void cx_test_assert(bool expr, const char* test,
                    const char* file, int line)
{
  if (!expr)
  {
    fprintf(stderr, "TEST FAILED: '%s' (file '%s', line %i))\n",
                    test, file, line);

    exit(EXIT_FAILURE);
  }
}

#endif // CX_TEST_SUPPORT


