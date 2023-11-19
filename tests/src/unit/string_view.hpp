// string_view.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_TEST_UNIT_STRING_VIEW_HPP_
#define _MJSTR_TEST_UNIT_STRING_VIEW_HPP_
#include <gtest/gtest.h>
#include <mjstr/string.hpp>

namespace mjx {
    namespace test {
        TEST(string_view, remove_prefix) {
            utf8_string_view _Str = "   trim me";

            // Test 1: Remove three characters, the new string should be "trim me".
            ASSERT_TRUE(_Str.remove_prefix(3));
            EXPECT_EQ(_Str.size(), 7);
            EXPECT_EQ(_Str, "trim me");

            // Test 2: Remove all characters, resulting in an empty string.
            ASSERT_TRUE(_Str.remove_prefix(_Str.size()));
            EXPECT_TRUE(_Str.empty());

            // Test 3: Attempt to remove one character from an empty string, which should fail.
            ASSERT_TRUE(_Str.empty());
            EXPECT_FALSE(_Str.remove_prefix(1));
        }

        TEST(string_view, remove_suffix) {
            utf8_string_view _Str = "trim me   ";

            // Test 1: Remove three characters, the new string should be "trim me".
            ASSERT_TRUE(_Str.remove_suffix(3));
            EXPECT_EQ(_Str.size(), 7);
            EXPECT_EQ(_Str, "trim me");

            // Test 2: Remove all characters, resulting in an empty string.
            ASSERT_TRUE(_Str.remove_suffix(_Str.size()));
            EXPECT_TRUE(_Str.empty());

            // Test 3: Attempt to remove one character from an empty string, which should fail.
            ASSERT_TRUE(_Str.empty());
            EXPECT_FALSE(_Str.remove_suffix(1));
        }

        TEST(string_view, find) {
            const utf8_string_view _Str = " long long int;";
            EXPECT_EQ(_Str.find("long"), 1);
            EXPECT_EQ(_Str.find("long", 2), 6);
            EXPECT_EQ(_Str.find(' '), 0);
            EXPECT_EQ(_Str.find('o', 1), 2);
            EXPECT_EQ(_Str.find("on"), 2);
            EXPECT_EQ(_Str.find("long double", 5, 4), 6);
            EXPECT_EQ(_Str.find("float"), utf8_string_view::npos);
        }

        TEST(string_view, rfind) {
            const utf8_string_view _Str = "AB AB AB";
            EXPECT_EQ(_Str.rfind("AB"), 6);
            EXPECT_EQ(_Str.rfind("ABCD", utf8_string_view::npos, 2), 6);
            EXPECT_EQ(_Str.rfind("AB", 5), 3);
            EXPECT_EQ(_Str.rfind("AB", 0), 0);
            EXPECT_EQ(_Str.rfind('B', 4), 4);
            EXPECT_EQ(_Str.rfind('C'), utf8_string_view::npos);
        }
    } // namespace test
} // namespace mjx

#endif // _MJSTR_TEST_UNIT_STRING_VIEW_HPP_