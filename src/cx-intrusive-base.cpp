// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */


#include "cx-intrusive.hpp"


CX_CONSTRUCTOR2(CX::IntrusiveBase::IntrusiveBase, : refcount_(0),
                                                    nascent_(true))

CX_ENDMETHOD



CX_DESTRUCTOR(CX::IntrusiveBase::~IntrusiveBase)

CX_ENDMETHOD



CX_CONSTMETHOD(void CX::IntrusiveBase::Adopted)

  ++refcount_;
  nascent_ = false;

CX_ENDMETHOD



CX_CONSTMETHOD(bool CX::IntrusiveBase::Disowned)

  CX_RETURN(nascent_ ? true : !--refcount_);

CX_ENDMETHOD

