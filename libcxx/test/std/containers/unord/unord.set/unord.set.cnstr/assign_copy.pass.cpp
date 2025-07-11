//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_set

// unordered_set& operator=(const unordered_set& u);

#include <unordered_set>
#include <algorithm>
#include <cassert>
#include <cfloat>
#include <cmath>
#include <cstddef>
#include <utility>

#include "test_macros.h"
#include "../../../test_compare.h"
#include "../../../test_hash.h"
#include "test_allocator.h"
#include "min_allocator.h"

int main(int, char**) {
  {
    typedef test_allocator<int> A;
    typedef std::unordered_set<int, test_hash<int>, test_equal_to<int>, A > C;
    typedef int P;
    P a[] = {P(1), P(2), P(3), P(4), P(1), P(2)};
    C c0(a, a + sizeof(a) / sizeof(a[0]), 7, test_hash<int>(8), test_equal_to<int>(9), A(10));
    C c(a, a + 2, 7, test_hash<int>(2), test_equal_to<int>(3), A(4));
    c = c0;
    LIBCPP_ASSERT(c.bucket_count() == 7);
    assert(c.size() == 4);
    assert(c.count(1) == 1);
    assert(c.count(2) == 1);
    assert(c.count(3) == 1);
    assert(c.count(4) == 1);
    assert(c.hash_function() == test_hash<int>(8));
    assert(c.key_eq() == test_equal_to<int>(9));
    assert(c.get_allocator() == A(4));
    assert(!c.empty());
    assert(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
    assert(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
    assert(fabs(c.load_factor() - (float)c.size() / c.bucket_count()) < FLT_EPSILON);
    assert(c.max_load_factor() == 1);
  }
  {
    typedef std::unordered_set<int> C;
    typedef int P;
    P a[] = {P(1), P(2), P(3), P(4), P(1), P(2)};
    C c(a, a + sizeof(a) / sizeof(a[0]));
    C* p = &c;
    c    = *p;
    assert(c.size() == 4);
    assert(std::is_permutation(c.begin(), c.end(), a));
  }
  {
    typedef other_allocator<int> A;
    typedef std::unordered_set<int, test_hash<int>, test_equal_to<int>, A > C;
    typedef int P;
    P a[] = {P(1), P(2), P(3), P(4), P(1), P(2)};
    C c0(a, a + sizeof(a) / sizeof(a[0]), 7, test_hash<int>(8), test_equal_to<int>(9), A(10));
    C c(a, a + 2, 7, test_hash<int>(2), test_equal_to<int>(3), A(4));
    c = c0;
    assert(c.bucket_count() >= 5);
    assert(c.size() == 4);
    assert(c.count(1) == 1);
    assert(c.count(2) == 1);
    assert(c.count(3) == 1);
    assert(c.count(4) == 1);
    assert(c.hash_function() == test_hash<int>(8));
    assert(c.key_eq() == test_equal_to<int>(9));
    assert(c.get_allocator() == A(10));
    assert(!c.empty());
    assert(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
    assert(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
    assert(fabs(c.load_factor() - (float)c.size() / c.bucket_count()) < FLT_EPSILON);
    assert(c.max_load_factor() == 1);
  }
#if TEST_STD_VER >= 11
  {
    typedef min_allocator<int> A;
    typedef std::unordered_set<int, test_hash<int>, test_equal_to<int>, A > C;
    typedef int P;
    P a[] = {P(1), P(2), P(3), P(4), P(1), P(2)};
    C c0(a, a + sizeof(a) / sizeof(a[0]), 7, test_hash<int>(8), test_equal_to<int>(9), A());
    C c(a, a + 2, 7, test_hash<int>(2), test_equal_to<int>(3), A());
    c = c0;
    LIBCPP_ASSERT(c.bucket_count() == 7);
    assert(c.size() == 4);
    assert(c.count(1) == 1);
    assert(c.count(2) == 1);
    assert(c.count(3) == 1);
    assert(c.count(4) == 1);
    assert(c.hash_function() == test_hash<int>(8));
    assert(c.key_eq() == test_equal_to<int>(9));
    assert(c.get_allocator() == A());
    assert(!c.empty());
    assert(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
    assert(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
    assert(fabs(c.load_factor() - (float)c.size() / c.bucket_count()) < FLT_EPSILON);
    assert(c.max_load_factor() == 1);
  }
#endif
  { // Test with std::pair, since we have some special handling for pairs inside __hash_table
    struct pair_hash {
      size_t operator()(std::pair<int, int> val) const TEST_NOEXCEPT { return val.first | val.second; }
    };

    std::pair<int, int> arr[] = {
        std::make_pair(1, 2), std::make_pair(2, 3), std::make_pair(3, 4), std::make_pair(4, 5)};
    std::unordered_set<std::pair<int, int>, pair_hash> a(arr, arr + 4);
    std::unordered_set<std::pair<int, int>, pair_hash> b;

    b = a;
    assert(a == b);
  }

  return 0;
}
