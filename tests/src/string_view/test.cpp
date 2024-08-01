// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjmem/exception.hpp>
#include <mjstr/string_view.hpp>

namespace mjx {
    TEST(string_view, front) {
        utf8_string_view _Str = "ABCDEF";
        EXPECT_EQ(_Str.front(), 'A');
        
        // remove "ABC" from _Str, the new string should be "DEF"
        _Str.remove_prefix(3);
        EXPECT_EQ(_Str.front(), 'D');
    }

    TEST(string_view, back) {
        utf8_string_view _Str = "ABCDEF";
        EXPECT_EQ(_Str.back(), 'F');

        // remove "DEF" from _Str, the new string should be "ABC"
        _Str.remove_suffix(3);
        EXPECT_EQ(_Str.back(), 'C');
    }

    TEST(string_view, remove_prefix) {
        utf8_string_view _Str = "   trim me";

        // remove three characters, the new string should be "trim me"
        _Str.remove_prefix(3);
        EXPECT_EQ(_Str.size(), 7);
        EXPECT_EQ(_Str, "trim me");

        // remove all characters, resulting in an empty string
        _Str.remove_prefix(_Str.size());
        EXPECT_TRUE(_Str.empty());
    }

    TEST(string_view, remove_suffix) {
        utf8_string_view _Str = "trim me   ";

        // remove three characters, the new string should be "trim me"
        _Str.remove_suffix(3);
        EXPECT_EQ(_Str.size(), 7);
        EXPECT_EQ(_Str, "trim me");

        // remove all characters, resulting in an empty string
        _Str.remove_suffix(_Str.size());
        EXPECT_TRUE(_Str.empty());
    }

    TEST(string_view, copy) {
        const utf8_string_view _Str = "ABCDEF";
        char _Buf[8]                = {'\0'};
        
        // copy the first four characters, _Buf should store "ABCD"
        _Str.copy(_Buf, 4);
        EXPECT_STREQ(_Buf, "ABCD");

        // copy four characters starting from the second one, _Buf should store "BCDE"
        _Str.copy(_Buf, 4, 1);
        EXPECT_STREQ(_Buf, "BCDE");

        // copy four characters starting from the third one, _Buf should store "CDEF"
        _Str.copy(_Buf, 4, 2);
        EXPECT_STREQ(_Buf, "CDEF");
    }

    TEST(string_view, substr) {
        const utf8_string_view _Str = "ABCDEF";
        EXPECT_EQ(_Str.substr(), _Str);
        EXPECT_EQ(_Str.substr(1), "BCDEF");
        EXPECT_EQ(_Str.substr(2, 3), "CDE");
        EXPECT_EQ(_Str.substr(4, 42), "EF");
        EXPECT_THROW(_Str.substr(10), resource_overrun); // offset out of range
    }

    TEST(string_view, compare) {
        EXPECT_LT(utf8_string_view{"abc"}.compare("abcd"), 0);
        EXPECT_GT(utf8_string_view{"abcd"}.compare("abc"), 0);
        EXPECT_EQ(utf8_string_view{"abc"}.compare("abc"), 0);
        EXPECT_EQ(utf8_string_view{""}.compare(""), 0);
    }

    TEST(string_view, starts_with) {
        const utf8_string_view _Str0 = "https://cppreference.com";
        EXPECT_TRUE(_Str0.starts_with(utf8_string_view{"http"}));
        EXPECT_FALSE(_Str0.starts_with(utf8_string_view{"ftp"}));
        
        const utf8_string_view _Str1 = "C++20";
        EXPECT_TRUE(_Str1.starts_with('C'));
        EXPECT_FALSE(_Str1.starts_with('J'));

        const utf8_string_view _Str2 = "string_view";
        EXPECT_TRUE(_Str2.starts_with("string"));
        EXPECT_FALSE(_Str2.starts_with("String"));
    }

    TEST(string_view, ends_with) {
        const utf8_string_view _Str0 = "https://cppreference.com";
        EXPECT_TRUE(_Str0.ends_with(utf8_string_view{"com"}));
        EXPECT_FALSE(_Str0.ends_with(utf8_string_view{"org"}));

        const utf8_string_view _Str1 = "C++20";
        EXPECT_TRUE(_Str1.ends_with('0'));
        EXPECT_FALSE(_Str1.ends_with('3'));

        const utf8_string_view _Str2 = "string_view";
        EXPECT_TRUE(_Str2.ends_with("view"));
        EXPECT_FALSE(_Str2.ends_with("View"));
    }

    TEST(string_view, contains) {
        const utf8_string_view _Str0 = "https://cppreference.com";
        EXPECT_TRUE(_Str0.contains(utf8_string_view{"cpp"}));
        EXPECT_FALSE(_Str0.contains(utf8_string_view{"php"}));

        const utf8_string_view _Str1 = "C++23";
        EXPECT_TRUE(_Str1.contains('+'));
        EXPECT_FALSE(_Str1.contains('-'));

        const utf8_string_view _Str2 = "utf8_string_view";
        EXPECT_TRUE(_Str2.contains("string"));
        EXPECT_FALSE(_Str2.contains("String"));
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
} // namespace mjx