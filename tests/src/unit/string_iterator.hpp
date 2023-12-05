// string_iterator.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_TEST_UNIT_STRING_ITERATOR_HPP_
#define _MJSTR_TEST_UNIT_STRING_ITERATOR_HPP_
#include <gtest/gtest.h>
#include <mjstr/string.hpp>

namespace mjx {
    namespace test {
        TEST(string_const_iterator, iteration) {
            const utf8_string& _Str = "utf8_string";
            char _Buf[12]           = {'\0'}; // _Str.size() + space for null-terminator
            size_t _Idx             = 0;
            for (const char _Ch : _Str) {
                _Buf[_Idx++] = _Ch;
            }

            EXPECT_EQ(_Str.compare(_Buf), 0);
        }

        TEST(string_const_iterator, dereference) {
            const utf8_string& _Str          = "utf8_string";
            utf8_string_const_iterator _Iter = _Str.begin();
            
            // Test 1: Dereference the default iterator, should point to 'u'.
            EXPECT_TRUE(*_Iter, 'u');

            // Test 2: Advance by 5 and dereference, should point to 's'.
            _Iter += 5;
            EXPECT_TRUE(*_Iter, 's');

            // Test 3: Advance by 3 and dereference, should point to 'i'.
            _Iter += 3;
            EXPECT_TRUE(*_Iter, 'i');

            // Test 4: Retreat by 7 and dereference, should point to 't'.
            _Iter -= 7;
            EXPECT_TRUE(*_Iter, 't');
        }

        TEST(string_const_iterator, element_access) {
            const utf8_string& _Str          = "utf8_string";
            utf8_string_const_iterator _Iter = _Str.begin();
        
            // Test 1: Access the fourth element, should point to '_'.
            EXPECT_EQ(_Iter[4], '_');
        
            // Test 2: Advance by 5 and access the second element, should point to 'r'.
            _Iter += 5;
            EXPECT_EQ(_Iter[2], 'r');

            // Test 3: Retreat by 2 and access the first element, should point to '8'.
            _Iter -= 2;
            EXPECT_EQ(_Iter[0], '8');
        }

        TEST(string_iterator, mutable_iteration) {
            utf8_string _Str = "utf8_string";
            for (char& _Ch : _Str) {
                _Ch = ::toupper(_Ch);
            }

            EXPECT_EQ(_Str, "UTF8_STRING");
        }

        TEST(string_iterator, mutable_element_access) {
            utf8_string _Str           = "utf8_string";
            utf8_string_iterator _Iter = _Str.begin();
            _Iter[0]                   = 'x';
            _Iter[4]                   = 'y';
            _Iter[10]                  = 'z';
            EXPECT_EQ(_Iter[0], 'x');
            EXPECT_EQ(_Iter[4], 'y');
            EXPECT_EQ(_Iter[10], 'z');
        }
    } // namespace test
} // namespace mjx

#endif // _MJSTR_TEST_UNIT_STRING_ITERATOR_HPP_