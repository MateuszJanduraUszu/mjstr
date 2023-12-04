// string_view_iterator.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_TEST_UNIT_STRING_VIEW_ITERATOR_HPP_
#define _MJSTR_TEST_UNIT_STRING_VIEW_ITERATOR_HPP_
#include <gtest/gtest.h>
#include <mjstr/string_view.hpp>

namespace mjx {
    namespace test {
        TEST(string_view_iterator, iteration) {
            const utf8_string_view _Str = "utf8_string_view";
            char _Buf[17]               = {'\0'}; // _Str.size() + space for null-terminator
            size_t _Idx                 = 0;
            for (const char _Ch : _Str) {
                _Buf[_Idx++] = _Ch;
            }

            EXPECT_EQ(_Str.compare(_Buf), 0);
        }

        TEST(string_view_iterator, dereference) {
            const utf8_string_view _Str     = "utf8_string_view";
            utf8_string_view_iterator _Iter = _Str.begin();

            // Test 1: Dereference the default iterator, should point to 'u'.
            EXPECT_EQ(*_Iter, 'u');

            // Test 2: Advance by 5 and dereference, should point to 's'.
            _Iter += 5;
            EXPECT_EQ(*_Iter, 's');

            // Test 3: Advance by 7 and dereference, should point to 'v'.
            _Iter += 7;
            EXPECT_EQ(*_Iter, 'v');
            
            // Test 4: Retreat by 4 and dereference, should point to 'i'.
            _Iter -= 4;
            EXPECT_EQ(*_Iter, 'i');
        }

        TEST(string_view_iterator, element_access) {
            const utf8_string_view _Str     = "utf8_string_view";
            utf8_string_view_iterator _Iter = _Str.begin();
            
            // Test 1: Access the fourth element, should point to '_'.
            EXPECT_EQ(_Iter[4], '_');

            // Test 2: Advance by 5 and access the seventh element, should point to 'v'.
            _Iter += 5;
            EXPECT_EQ(_Iter[7], 'v');

            // Test 3: Retreat by 2 and access the first element, should point to '8'.
            _Iter -= 2;
            EXPECT_EQ(_Iter[0], '8');
        }
    } // namespace test
} // namespace mjx

#endif // _MJSTR_TEST_UNIT_STRING_VIEW_ITERATOR_HPP_