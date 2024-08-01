// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjstr/string.hpp>

namespace mjx {
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
            
        // dereference the default iterator, should point to 'u'
        EXPECT_EQ(*_Iter, 'u');

        // advance by 5 and dereference, should point to 's'
        _Iter += 5;
        EXPECT_EQ(*_Iter, 's');

        // advance by 3 and dereference, should point to 'i'
        _Iter += 3;
        EXPECT_EQ(*_Iter, 'i');

        // retreat by 7 and dereference, should point to 't'
        _Iter -= 7;
        EXPECT_EQ(*_Iter, 't');
    }

    TEST(string_const_iterator, element_access) {
        const utf8_string& _Str          = "utf8_string";
        utf8_string_const_iterator _Iter = _Str.begin();
        
        // access the fourth element, should point to '_'
        EXPECT_EQ(_Iter[4], '_');
        
        // advance by 5 and access the second element, should point to 'r'
        _Iter += 5;
        EXPECT_EQ(_Iter[2], 'r');

        // retreat by 2 and access the first element, should point to '8'
        _Iter -= 2;
        EXPECT_EQ(_Iter[0], '8');
    }

    TEST(string_iterator, mutable_iteration) {
        utf8_string _Str = "utf8_string";
        for (char& _Ch : _Str) {
            _Ch = ::towupper(_Ch);
        }

        EXPECT_EQ(_Str, "UTF8_STRING");
    }

    TEST(string_iterator, mutable_element_access) {
        utf8_string _Str           = "utf8_string";
        utf8_string_iterator _Iter = _Str.begin();
        _Iter[0]                   = 'x';
        _Iter[4]                   = 'y';
        _Iter[10]                  = 'z';
        EXPECT_EQ(_Str, "xtf8ystrinz");
        EXPECT_EQ(_Iter[0], 'x');
        EXPECT_EQ(_Iter[4], 'y');
        EXPECT_EQ(_Iter[10], 'z');
    }
} // namespace mjx