// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjstr/string_view.hpp>

namespace mjx {
    TEST(string_view_iterator, iteration) {
        const utf8_string_view _Str = "utf8_string_view";
        char _Buf[17]               = {'\0'}; // _Str.size() + space for null terminator
        size_t _Idx                 = 0;
        for (const char _Ch : _Str) {
            _Buf[_Idx++] = _Ch;
        }

        EXPECT_EQ(_Str.compare(_Buf), 0);
    }

    TEST(string_view_iterator, dereference) {
        const utf8_string_view _Str     = "utf8_string_view";
        utf8_string_view_iterator _Iter = _Str.begin();

        // dereference the default iterator, should point to 'u'
        EXPECT_EQ(*_Iter, 'u');

        // advance by 5 and dereference, should point to 's'
        _Iter += 5;
        EXPECT_EQ(*_Iter, 's');

        // advance by 7 and dereference, should point to 'v'
        _Iter += 7;
        EXPECT_EQ(*_Iter, 'v');

        // retreat by 4 and dereference, should point to 'i'
        _Iter -= 4;
        EXPECT_EQ(*_Iter, 'i');
    }

    TEST(string_view_iterator, element_access) {
        const utf8_string_view _Str     = "utf8_string_view";
        utf8_string_view_iterator _Iter = _Str.begin();

        // access the fourth element, should point to '_'
        EXPECT_EQ(_Iter[4], '_');

        // advance by 5 and access the seventh element, should point to 'v'
        _Iter += 5;
        EXPECT_EQ(_Iter[7], 'v');

        // retreat by 2 and access the first element, should point to '8'
        _Iter -= 2;
        EXPECT_EQ(_Iter[0], '8');
    }
} // namespace mjx