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
            const utf8_string_view _Str = "utf8_string_view";
            auto _Iter                  = _Str.begin();
            EXPECT_EQ(::strcmp(_Iter.operator->(), "utf8_string_view"), 0);

            _Iter += 5;
            EXPECT_EQ(::strcmp(_Iter.operator->(), "string_view"), 0);

            _Iter += 7;
            EXPECT_EQ(::strcmp(_Iter.operator->(), "view"), 0);
            
            _Iter -= 4;
            EXPECT_EQ(::strcmp(_Iter.operator->(), "ing_view"), 0);
        }

        TEST(string_view_iterator, element_access) {
            const utf8_string_view _Str = "utf8_string_view";
            auto _Iter                  = _Str.begin();
            EXPECT_EQ(_Iter[4], '_');

            _Iter += 5;
            EXPECT_EQ(_Iter[4], 'n');

            _Iter -= 2;
            EXPECT_EQ(_Iter[4], 'r');
        }
    } // namespace test
} // namespace mjx

#endif // _MJSTR_TEST_UNIT_STRING_VIEW_ITERATOR_HPP_