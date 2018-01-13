// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :

/*
 * Copyright (c) 2016-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#include "cx-test-support.hpp"
#include "cx-endian.hpp"

#include <iostream>

void Test_HOST()
{
  if (CX_HOST_IS_BIG_ENDIAN)
  {
    printf("This test assumes that the host is little-endian.\n");
    printf("Someone needs to update it for your system.  Sorry.\n");
    exit(EXIT_FAILURE);
  }
}


template <typename T, typename U>
void cx_test_endianness(char const* test, T value, U expect)
{
  if (value != expect)
  {
    std::cerr << "TEST FAILED: '" << test << "'\n";
    std::cerr << "  expected 0x" << std::hex << expect << ", got 0x" << value << "\n";
    exit(EXIT_FAILURE);
  }
}

#define CX_TEST_ENDIANNESS(test,expect) cx_test_endianness(#test,test,expect)

void Test_16BIT()
{
  CX_TEST_ENDIANNESS(CX::Endian::betoh((U16)0x0102), 0x0201);
  CX_TEST_ENDIANNESS(CX::Endian::betoh((S16)0x0203), 0x0302);

  CX_TEST_ENDIANNESS(CX::Endian::letoh((U16)0x0304), 0x0304);
  CX_TEST_ENDIANNESS(CX::Endian::letoh((S16)0x0405), 0x0405);

  CX_TEST_ENDIANNESS(CX::Endian::htole((U16)0x0506), 0x0506);
  CX_TEST_ENDIANNESS(CX::Endian::htole((S16)0x0607), 0x0607);

  CX_TEST_ENDIANNESS(CX::Endian::htobe((U16)0x0708), 0x0807);
  CX_TEST_ENDIANNESS(CX::Endian::htobe((S16)0x0809), 0x0908);
}


void Test_32BIT()
{
  CX_TEST_ENDIANNESS(CX::Endian::betoh((U32)0x01020304), 0x04030201);
  CX_TEST_ENDIANNESS(CX::Endian::betoh((S32)0x02030405), 0x05040302);

  CX_TEST_ENDIANNESS(CX::Endian::letoh((U32)0x03040506), 0x03040506);
  CX_TEST_ENDIANNESS(CX::Endian::letoh((S32)0x04050607), 0x04050607);

  CX_TEST_ENDIANNESS(CX::Endian::htole((U32)0x05060708), 0x05060708);
  CX_TEST_ENDIANNESS(CX::Endian::htole((S32)0x06070809), 0x06070809);

  CX_TEST_ENDIANNESS(CX::Endian::htobe((U32)0x0708090a), 0x0a090807);
  CX_TEST_ENDIANNESS(CX::Endian::htobe((S32)0x08090a0b), 0x0b0a0908);
}


void Test_64BIT()
{
  CX_TEST_ENDIANNESS(CX::Endian::betoh((U64)0x01020304), 0x0403020100000000);
  CX_TEST_ENDIANNESS(CX::Endian::betoh((S64)0x02030405), 0x0504030200000000);

  CX_TEST_ENDIANNESS(CX::Endian::letoh((U64)0x03040506), 0x03040506);
  CX_TEST_ENDIANNESS(CX::Endian::letoh((S64)0x04050607), 0x04050607);

  CX_TEST_ENDIANNESS(CX::Endian::htole((U64)0x05060708), 0x05060708);
  CX_TEST_ENDIANNESS(CX::Endian::htole((S64)0x06070809), 0x06070809);

  CX_TEST_ENDIANNESS(CX::Endian::htobe((U64)0x0708090a), 0x0a09080700000000);
  CX_TEST_ENDIANNESS(CX::Endian::htobe((S64)0x08090a0b), 0x0b0a090800000000);
}


int main(int argc, char** argv)
{
  Test_HOST();

  Test_16BIT();
  Test_32BIT();
  Test_64BIT();

  exit(EXIT_SUCCESS);
}

