// conversion.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_TEST_UNIT_CONVERSION_HPP_
#define _MJSTR_TEST_UNIT_CONVERSION_HPP_
#include <gtest/gtest.h>
#include <mjstr/conversion.hpp>

namespace mjx {
    namespace test {
        TEST(utf8_to_unicode, empty_string) {
            EXPECT_TRUE(::mjx::to_unicode_string("").empty());
        }

        TEST(utf8_to_unicode, no_unicode_characters) {
            EXPECT_EQ(::mjx::to_unicode_string("No Unicode characters."), L"No Unicode characters.");
            EXPECT_EQ(::mjx::to_unicode_string("Sentence in UTF-8."), L"Sentence in UTF-8.");
            EXPECT_EQ(::mjx::to_unicode_string("No conversion from UTF-8 to Unicode required."),
                L"No conversion from UTF-8 to Unicode required.");
        }

        TEST(utf8_to_unicode, unicode_characters) {
            EXPECT_EQ(::mjx::to_unicode_string(
                "\xE7\xB0\xA1\xE5\x96\xAE\xE7\x9A\x84\xE4\xB8\xAD\xE6\x96\x87\xE5\x8F\xA5\xE5\xAD\x90\xE3\x80\x82"),
                L"簡單的中文句子。"
            );
            EXPECT_EQ(::mjx::to_unicode_string(
                "\xD8\xAC\xD9\x85\xD9\x84\xD8\xA9\x20\xD8\xA8\xD8\xB3\xD9\x8A\xD8\xB7\xD8\xA9\x20\xD8\xA3\xD8\xAE"
                "\xD8\xB1\xD9\x89\x20\xD8\xA8\xD8\xA7\xD9\x84\xD9\x84\xD8\xBA\xD8\xA9\x20\xD8\xA7\xD9\x84\xD8\xB9"
                "\xD8\xB1\xD8\xA8\xD9\x8A\xD8\xA9\x2E"),
                L"جملة بسيطة أخرى باللغة العربية."
            );
            EXPECT_EQ(::mjx::to_unicode_string(
                "\xD0\x9F\xD0\xBE\xD1\x81\xD0\xBB\xD0\xB5\xD0\xB4\xD0\xBD\xD0\xB5\xD0\xB5\x20\xD0\xBF\xD1\x80\xD0"
                "\xB5\xD0\xB4\xD0\xBB\xD0\xBE\xD0\xB6\xD0\xB5\xD0\xBD\xD0\xB8\xD0\xB5\x2C\x20\xD0\xBD\xD0\xB0\x20"
                "\xD1\x8D\xD1\x82\xD0\xBE\xD1\x82\x20\xD1\x80\xD0\xB0\xD0\xB7\x20\xD0\xBD\xD0\xB0\x20\xD1\x80\xD1"
                "\x83\xD1\x81\xD1\x81\xD0\xBA\xD0\xBE\xD0\xBC\x20\xD1\x8F\xD0\xB7\xD1\x8B\xD0\xBA\xD0\xB5\x2E"),
                L"Последнее предложение, на этот раз на русском языке."
            );
        }

        TEST(unicode_to_utf8, empty_string) {
            EXPECT_TRUE(::mjx::to_utf8_string(L"").empty());
        }

        TEST(unicode_to_utf8, no_unicode_characters) {
            EXPECT_EQ(::mjx::to_utf8_string(L"No Unicode chars."), "No Unicode chars.");
            EXPECT_EQ(::mjx::to_utf8_string(L"Sentence in UTF-8."), "Sentence in UTF-8.");
            EXPECT_EQ(::mjx::to_utf8_string(L"No conversion from Unicode to UTF-8 required."),
                "No conversion from Unicode to UTF-8 required.");
        }

        TEST(unicode_to_utf8, unicode_characters) {
            EXPECT_EQ(::mjx::to_utf8_string(L"簡單的中文句子。"),
                "\xE7\xB0\xA1\xE5\x96\xAE\xE7\x9A\x84\xE4\xB8\xAD\xE6\x96\x87\xE5\x8F\xA5\xE5\xAD\x90\xE3\x80\x82"
            );
            EXPECT_EQ(::mjx::to_utf8_string(L"جملة بسيطة أخرى باللغة العربية."),
                "\xD8\xAC\xD9\x85\xD9\x84\xD8\xA9\x20\xD8\xA8\xD8\xB3\xD9\x8A\xD8\xB7\xD8\xA9\x20\xD8\xA3\xD8\xAE"
                "\xD8\xB1\xD9\x89\x20\xD8\xA8\xD8\xA7\xD9\x84\xD9\x84\xD8\xBA\xD8\xA9\x20\xD8\xA7\xD9\x84\xD8\xB9"
                "\xD8\xB1\xD8\xA8\xD9\x8A\xD8\xA9\x2E"
            );
            EXPECT_EQ(::mjx::to_utf8_string(L"Последнее предложение, на этот раз на русском языке."),
                "\xD0\x9F\xD0\xBE\xD1\x81\xD0\xBB\xD0\xB5\xD0\xB4\xD0\xBD\xD0\xB5\xD0\xB5\x20\xD0\xBF\xD1\x80\xD0"
                "\xB5\xD0\xB4\xD0\xBB\xD0\xBE\xD0\xB6\xD0\xB5\xD0\xBD\xD0\xB8\xD0\xB5\x2C\x20\xD0\xBD\xD0\xB0\x20"
                "\xD1\x8D\xD1\x82\xD0\xBE\xD1\x82\x20\xD1\x80\xD0\xB0\xD0\xB7\x20\xD0\xBD\xD0\xB0\x20\xD1\x80\xD1"
                "\x83\xD1\x81\xD1\x81\xD0\xBA\xD0\xBE\xD0\xBC\x20\xD1\x8F\xD0\xB7\xD1\x8B\xD0\xBA\xD0\xB5\x2E"
            );
        }
    } // namespace test
} // namespace mjx

#endif // _MJSTR_TEST_UNIT_CONVERSION_HPP_