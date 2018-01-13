// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :

/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#include "cx-test-support.hpp"
#include "cx-factory.hpp"

#include <string.h>
#include <iostream>

class TestBase
{
  public:
    TestBase() {};
    virtual std::string Text() = 0;
};

class Test1Class: public TestBase
{
  public:
    Test1Class(const std::string& text): TestBase(), text_(text) {}
    std::string Text() { return std::string("test1: ") + text_; }

  private:
    std::string text_;
};

class TEST2Class: public TestBase
{
  public:
    TEST2Class(const std::string& text): TestBase(), text_(text) {}
    std::string Text() { return std::string("TEST2: ") + text_; }

  private:
    std::string text_;
};


int main(int argc, char** argv)
{
  CX::Factory<TestBase, uint32_t, std::string> testFactory;

  testFactory.Define<Test1Class>(1);
  testFactory.Define<TEST2Class>(42);

  TestBase* foo = testFactory.New(1, "plugh");
  TestBase* bar = testFactory.New(42, "xyzzy");

  CX_TEST_ASSERT(0 == strcmp(foo->Text().c_str(), "test1: plugh"));
  CX_TEST_ASSERT(0 == strcmp(bar->Text().c_str(), "TEST2: xyzzy"));

  exit(EXIT_SUCCESS);
}

