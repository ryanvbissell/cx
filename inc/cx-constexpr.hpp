// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2020, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 *
 *
 * 'FOLLY' portions Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CX_CONSTEXPR_HPP
#define CX_CONSTEXPR_HPP

namespace CX {

#define FOLLY 1
#if FOLLY
// shamelessly stolen from  https://github.com/facebook/folly
namespace detail {
  template <typename T>
  constexpr T constexpr_log2(T a, T e) {
    return e == T(1) ? a : constexpr_log2(a + T(1), e / T(2));
  }
}  // namespace 'detail'

template <typename T>
constexpr T constexpr_log2(T t) {
  return detail::constexpr_log2(T(0), t);
}
#endif // FOLLY

} // namespace 'CX'

#endif // CX_CONSTEXPR_HPP

