// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "fib"
#define CX_TESTING

#include "cx-test-support.hpp"
#include "cx-tracedebug.hpp"
#include "cx-exceptions.hpp"


class Fib
{
  public:
    Fib();
    void Sequence(U64 n, U64 max);

  private:
    U64 fib(U64 n);
};


CX_CONSTRUCTOR0(Fib::Fib)
  CX_TRACEOUT("%s\n",
              "(This line will print whenever tracing is enabled.)");
  CX_TOPICOUT(fib:misc, "This constructor doesn't do anything.\n");
CX_ENDMETHOD


CX_METHOD(U64 Fib::fib, U64 n)

  // TODO: PRIu64 version doesn't work with g++ version 4.8.4
  //CX_DEBUGOUT("n=" PRIu64 "\n", n);
  CX_TOPICOUT(fib:args, "n=%lu\n", n);
  switch(n)
  {
    case 0: CX_RETURN(0);
    case 1: CX_RETURN(1);
  }
  CX_RETURN(fib(n-1) + fib(n-2));

CX_ENDMETHOD



CX_METHOD(void Fib::Sequence, U64 n, U64 max)

  U64 result;

  // TODO: PRIu64 version doesn't work with g++ version 4.8.4
  //CX_TOPICOUT(fib:args, "n=" PRIu64 ", max=" PRIu64 "\n", n, max);
  CX_TOPICOUT(fib:args, "n=%lu, max=%lu\n", n, max);
  result = fib(max - n);
  printf("%" PRIu64 "\n", result);
  if (result == 8)
    CX_THROW( CX::Exception,
              CX::Error::NONE,
              "We've reached 8 in the fibonacci sequence");

  if (n > 0)
  {
    Sequence(n-1, max);
  }

  CX_WARNING("This is an example warning.\n\n");

CX_ENDMETHOD



CX_FUNCTION(void fib_sequence, U64 n)

  Fib fib;

  fib.Sequence(n, n);

  CX_PWARNING(true,
              "This warning is predicated, and should print\n");
  CX_PWARNING(false,
      "(This warning is predicated, but should not print)\n");
  CX_WARNING("\n");

  CX_ERROROUT("%s\n", "This is an example CX_ERROROUT() invocation.");
  CX_ERROROUT("(Although, we swapped the stdout & stderr file \n");
  CX_ERROROUT("handles, so it will actually appear on stdout)\n\n");

  CX_PERROROUT(true, "This error is predicated, and should print\n");
  CX_PERROROUT(false, "(This predicated error should not print)\n");

CX_ENDFUNCTION



CX_FUNCTION(int main, int argc, char** argv)

  // TODO:  command-line arg(s) to control mixing of
  //        CX::debugout and CX::errorout

  CX_WARNING("This is an example warning.\n\n");

  CX_TRY
  {
    fib_sequence(6);
  }
  CX_CATCH(const CX::Exception& e)
  {
    e.StdError();
  }
  CX_ENDTRY

  CX_DEBUGOUT("%s\n", "We are back in main().");

  CX_TOPICOUT(fib:misc, "CX_TRACE was '%s'\n", CX::get_traceenv());
  CX_TOPICOUT(fib:misc, "CX_TOPICS was '%s'\n", CX::get_topicenv());
  CX_TOPICOUT(fib:misc, "CX_TRACEFILE was '%s'\n", CX::get_tracefile());

  CX_TRACEOUT("Yet another CX_TRACEOUT example\n");

CX_ENDFUNCTION


