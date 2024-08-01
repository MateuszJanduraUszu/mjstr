// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjstr/char_traits.hpp>
#include <mjstr/string_view.hpp>

namespace mjx {
    TEST(char_traits, compatible_element) {
        // test compatibility with char/byte_t/wchar_t
        EXPECT_TRUE(compatible_element<char>);
        EXPECT_TRUE(compatible_element<byte_t>);
        EXPECT_TRUE(compatible_element<wchar_t>);

        // test incompatibility with char8_t/char16_t/char32_t
        EXPECT_FALSE(compatible_element<char8_t>);
        EXPECT_FALSE(compatible_element<char16_t>);
        EXPECT_FALSE(compatible_element<char32_t>);
    }

    TEST(char_traits, assign) {
        using _Traits = char_traits<char>;
        char _Buf[5]  = {'\0'};
        
        // set the first two characters to "x", _Buf should store "XX"
        _Traits::assign(_Buf, 2, 'X');
        EXPECT_STREQ(_Buf, "XX");

        // set the first four characters to "Y", _Buf should store "YYYY"
        _Traits::assign(_Buf, 4, 'Y');
        EXPECT_STREQ(_Buf, "YYYY");
    }

    TEST(char_traits, eq) {
        using _Traits = char_traits<char>;
        EXPECT_TRUE(_Traits::eq("string", "string", 6));
        EXPECT_TRUE(_Traits::eq("string", "string_view", 6));
        EXPECT_FALSE(_Traits::eq("String", "string", 6));
        EXPECT_FALSE(_Traits::eq("String_view", "string_view", 6));
    }

    TEST(char_traits, move) {
        using _Traits = char_traits<char>;
        char _Buf[5]  = {'\0'};
        _Traits::copy(_Buf, "ABCD", 4);
        _Traits::move(_Buf, _Buf, 4); // should handle overlapping
        EXPECT_STREQ(_Buf, "ABCD");
    }

    TEST(char_traits, copy) {
        using _Traits = char_traits<char>;
        char _Buf[5]  = {'\0'};
    
        // assign "ABCD" to the buffer, _Buf should store "ABCD"
        _Traits::copy(_Buf, "ABCD", 4);
        EXPECT_EQ(utf8_string_view(_Buf, 4), "ABCD");

        // assign "EFG" to the buffer, _Buf should store "EFG"
        _Traits::copy(_Buf, "EFG", 3);
        EXPECT_EQ(utf8_string_view(_Buf, 3), "EFG");

        // assign "Z" to the buffer, _Buf should store "Z"
        _Traits::copy(_Buf, "Z", 1);
        EXPECT_EQ(utf8_string_view(_Buf, 1), "Z");
    }

    TEST(char_traits, compare) {
        using _Traits = char_traits<char>;
        EXPECT_EQ(_Traits::compare("utf8_string", 0, "unicode_string", 0), 0); // empty strings are always equal
        EXPECT_EQ(_Traits::compare("string", 6, "string", 6), 0);
        EXPECT_EQ(_Traits::compare("string", 6, "string_view", 6), 0);
        EXPECT_EQ(_Traits::compare("String", 6, "string", 6), -1);
        EXPECT_EQ(_Traits::compare("BCDE", 4, "ABCD", 4), 1);
    }

    TEST(char_traits, length) {
        using _Traits = char_traits<char>;
        EXPECT_EQ(_Traits::length("foo"), 3);
        EXPECT_EQ(_Traits::length("booo"), 4);
    }

    TEST(char_traits, find) {
        using _Traits          = char_traits<char>;
        constexpr size_t _Npos = static_cast<size_t>(-1);

        // search for a character
        EXPECT_EQ(_Traits::find("C++20", 5, '2'), 3);
        EXPECT_EQ(_Traits::find("C++23", 5, '6'), _Npos);
    
        // search for a substring
        EXPECT_EQ(_Traits::find("string_view", 11, "view", 4), 7);
        EXPECT_EQ(_Traits::find("string_view", 11, "utf8", 4), _Npos);
    }

    TEST(char_traits, rfind) {
        using _Traits          = char_traits<char>;
        constexpr size_t _Npos = static_cast<size_t>(-1);
    
        // search for a character
        EXPECT_EQ(_Traits::rfind("AB AB AB", 8, 'B'), 7);
        EXPECT_EQ(_Traits::rfind("AB AB AB", 8, 'C'), _Npos);

        // search for a substring
        EXPECT_EQ(_Traits::rfind("AB CD AB CD", 11, "AB", 2), 6);
        EXPECT_EQ(_Traits::rfind("AB CD AB CD", 11, "ABC", 3), _Npos);
    }
} // namespace mjx