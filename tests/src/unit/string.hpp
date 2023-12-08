// string.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_TEST_UNIT_STRING_HPP_
#define _MJSTR_TEST_UNIT_STRING_HPP_
#include <gtest/gtest.h>
#include <mjstr/string.hpp>

namespace mjx {
    namespace test {
        TEST(string, assign) {
            utf8_string _Str;
        
            // Test 1: Assign a char 4 times.
            _Str.assign(4, '=');
            EXPECT_EQ(_Str, "====");

            // Test 2: Assign a string.
            string _Temp = "Exemplary";
            _Str.assign(_Temp);
            EXPECT_EQ(_Str, _Temp);

            // Test 3: Assign an r-value string.
            _Str.assign(utf8_string{"C++ by "} + "example");
            EXPECT_EQ(_Str, "C++ by example");

            // Test 4: Assign a C-style string.
            const char* const _Cstr = "C-style string";
            _Str.assign(_Cstr);
            EXPECT_EQ(_Str, _Cstr);
        }

        TEST(string, append) {
            const utf8_string& _Str = "string";
            const char* const _Ptr  = "C-string";
            utf8_string _Whole_str;

            // Test 1: Append a char 3 times. 
            _Whole_str.append(3, '*');
            EXPECT_EQ(_Whole_str, "***");
 
            // Test 2: Append a whole string.
            _Whole_str.push_back(' ');
            _Whole_str.append(_Str);
            EXPECT_EQ(_Whole_str, "*** string");

            // Test 3: Append a whole C-string.
            _Whole_str.push_back(' ');
            _Whole_str.append(_Ptr);
            EXPECT_EQ(_Whole_str, "*** string C-string");
        }

        TEST(string, reserve) {
            utf8_string _Str;

            // Test 1: Initial capacity.
            EXPECT_EQ(_Str.capacity(), 15); // SSO active, small string capacity is 16 bytes (with null-terminator)
            
            // Test 2: Capacity for 10 characters.
            _Str.reserve(10);
            EXPECT_EQ(_Str.capacity(), 15); // requested less capacity than current, nothing has changed
        
            // Test 3: Capacity for 100 characters.
            _Str.reserve(100);
            constexpr size_t _Align        = 2 * sizeof(void*);
            constexpr size_t _Mask         = _Align - 1;
            constexpr size_t _Aligned_size = (100 + _Mask) & ~_Mask;
            EXPECT_EQ(_Str.capacity(), _Aligned_size - 1);
        }

        TEST(string, resize) {
            utf8_string _Str(20, 'x');

            // Test 1: Increase string size.
            _Str.resize(30, 'y');
            EXPECT_EQ(_Str.size(), 30);
            EXPECT_EQ(_Str, utf8_string(20, 'x') + utf8_string(10, 'y'));
        
            // Test 2: Decrease string size.
            _Str.resize(10);
            EXPECT_EQ(_Str.size(), 10);
            EXPECT_EQ(_Str, utf8_string(10, 'x'));
        }

        TEST(string, erase) {
            utf8_string _Str = "This Is An Example";

            // Test 1: Erase " An".
            _Str.erase(7, 3);
            EXPECT_EQ(_Str, "This Is Example");

            // Test 2: Erase first " ".
            _Str.erase(_Str.begin() + _Str.find(' '));
            EXPECT_EQ(_Str, "ThisIs Example");

            // Test 3: Trim from " " to the end of the string.
            _Str.erase(_Str.find(' '));
            EXPECT_EQ(_Str, "ThisIs");

            // Test 4: Erase "Is".
            const utf8_string_const_iterator _First = _Str.begin() + _Str.find('s');
            _Str.erase(_First, _First + 2);
            EXPECT_EQ(_Str, "This");
        }

        TEST(string, insert) {
            utf8_string _Str = "xmplr";

            // Test 1: Insert "E" at the beginning.
            _Str.insert(0, 1, 'E');
            EXPECT_EQ(_Str, "Exmplr");

            // Test 2: Insert "e" at the second place.
            _Str.insert(2, "e");
            EXPECT_EQ(_Str, "Exemplr");

            // Test 3: Insert "a" at the sixth place.
            _Str.insert(6, utf8_string{"a"});
            EXPECT_EQ(_Str, "Exemplar");
            
            // Test 4: Insert " is an example." at the end.
            _Str.insert(8, " is an example.");
            EXPECT_EQ(_Str, "Exemplar is an example.");
        
            // Test 5: Insert ":" after "Exemplar".
            _Str.insert(_Str.begin() + 14, ':');
            EXPECT_EQ(_Str, "Exemplar is an: example.");
        }

        TEST(string, replace) {
            utf8_string _Str = "Hello World!";

            // Test 1: Replace "Hello" with "Hi".
            _Str.replace(0, 5, "Hi");
            EXPECT_EQ(_Str, "Hi World!");

            { // Test 2: Replace "World" with "John".
                const auto& _Iter = _Str.begin() + _Str.find('W');
                _Str.replace(_Iter, _Iter + 5, "John");
                EXPECT_EQ(_Str, "Hi John!");
            }

            { // Test 3: Replace "!" with "...".
                const auto& _Iter = _Str.begin() + _Str.size() - 1;
                _Str.replace(_Iter, _Iter + 1, 3, '.');
                EXPECT_EQ(_Str, "Hi John...");
            }
        }
    } // namespace test
} // namespace mjx

#endif // _MJSTR_TEST_UNIT_STRING_HPP_