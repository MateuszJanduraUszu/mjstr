// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjstr/conversion.hpp>

#ifdef _MJX_MSVC
#pragma warning(push, 1)
#pragma warning(disable : 4566) // C4566: character cannot be represented in the current code page
#endif // _MJX_MSVC

namespace mjx {
    inline void test_unicode_utf8(const wchar_t* const _Unicode, const char* const _Utf8) {
        // encode _Unicode to UTF-8 and compare with the expected bytes, then decode
        // the encoded bytes back to Unicode and compare with the original string
        const utf8_string& _Utf8_cvt   = ::mjx::to_utf8_string(_Unicode);
        const unicode_string& _Unc_cvt = ::mjx::to_unicode_string(_Utf8_cvt);
        EXPECT_EQ(_Utf8_cvt, _Utf8);
        EXPECT_EQ(_Unc_cvt, _Unicode);
    }

    inline void test_invalid_utf8(const char* const _Bytes) {
        // attempt to decode _Bytes back to Unicode, since _Bytes are invalid,
        // to_unicode_string() is expected to fail and return an empty string
        EXPECT_TRUE(::mjx::to_unicode_string(_Bytes).empty());
    }

    TEST(unicode_utf8, empty) {
        test_unicode_utf8(L"", "");
    }

    TEST(unicode_utf8, one_byte) {
        test_unicode_utf8(L"Ab$3z", "\x41\x62\x24\x33\x7A");
        test_unicode_utf8(L"Mn*9x", "\x4D\x6E\x2A\x39\x78");
        test_unicode_utf8(L"Pq!4y", "\x50\x71\x21\x34\x79");
        test_unicode_utf8(L"Rs#5w", "\x52\x73\x23\x35\x77");
        test_unicode_utf8(L"Tu&6v", "\x54\x75\x26\x36\x76");
    }

    TEST(unicode_utf8, two_bytes) {
        test_unicode_utf8(L"©éăđħ", "\xC2\xA9\xC3\xA9\xC4\x83\xC4\x91\xC4\xA7");
        test_unicode_utf8(L"ıłœťŷ", "\xC4\xB1\xC5\x82\xC5\x93\xC5\xA5\xC5\xB7");
        test_unicode_utf8(L"ƈƙơƴǅ", "\xC6\x88\xC6\x99\xC6\xA1\xC6\xB4\xC7\x85");
        test_unicode_utf8(L"ǖǧǸȉȚ", "\xC7\x96\xC7\xA7\xC7\xB8\xC8\x89\xC8\x9A");
        test_unicode_utf8(L"ȫȼɍɞɯ", "\xC8\xAB\xC8\xBC\xC9\x8D\xC9\x9E\xC9\xAF");
    }

    TEST(unicode_utf8, three_bytes) {
        test_unicode_utf8(L"กขฃคฅ", "\xE0\xB8\x81\xE0\xB8\x82\xE0\xB8\x83\xE0\xB8\x84\xE0\xB8\x85");
        test_unicode_utf8(L"अआइईउ", "\xE0\xA4\x85\xE0\xA4\x86\xE0\xA4\x87\xE0\xA4\x88\xE0\xA4\x89");
        test_unicode_utf8(L"你好世界人", "\xE4\xBD\xA0\xE5\xA5\xBD\xE4\xB8\x96\xE7\x95\x8C\xE4\xBA\xBA");
        test_unicode_utf8(L"あいうえお", "\xE3\x81\x82\xE3\x81\x84\xE3\x81\x86\xE3\x81\x88\xE3\x81\x8A");
        test_unicode_utf8(L"അആഇഈഉ", "\xE0\xB4\x85\xE0\xB4\x86\xE0\xB4\x87\xE0\xB4\x88\xE0\xB4\x89");
    }

    TEST(unicode_utf8, four_bytes) {
        test_unicode_utf8(L"😀😁😂😃😄",
            "\xF0\x9F\x98\x80\xF0\x9F\x98\x81\xF0\x9F\x98\x82\xF0\x9F\x98\x83\xF0\x9F\x98\x84");
        test_unicode_utf8(L"🚀🚁🚂🚃🚄",
            "\xF0\x9F\x9A\x80\xF0\x9F\x9A\x81\xF0\x9F\x9A\x82\xF0\x9F\x9A\x83\xF0\x9F\x9A\x84");
        test_unicode_utf8(L"💩💪💫💬💭",
            "\xF0\x9F\x92\xA9\xF0\x9F\x92\xAA\xF0\x9F\x92\xAB\xF0\x9F\x92\xAC\xF0\x9F\x92\xAD");
        test_unicode_utf8(L"🌍🌎🌏🌐🌑",
            "\xF0\x9F\x8C\x8D\xF0\x9F\x8C\x8E\xF0\x9F\x8C\x8F\xF0\x9F\x8C\x90\xF0\x9F\x8C\x91");
        test_unicode_utf8(L"🎤🎥🎦🎧🎨",
            "\xF0\x9F\x8E\xA4\xF0\x9F\x8E\xA5\xF0\x9F\x8E\xA6\xF0\x9F\x8E\xA7\xF0\x9F\x8E\xA8");
    }

    TEST(invalid_utf8, two_bytes) {
        test_invalid_utf8("\xC0\x80\xC1\x81\xC2\x82");
        test_invalid_utf8("\xC2\x20\xC3\x30\xC4\x40");
        test_invalid_utf8("\xC2\xE3\xC3\xF4\xC4");
    }

    TEST(invalid_utf8, three_bytes) {
        test_invalid_utf8("\xE0\x80\x80\xE0\x81\x81\xE0\x82\x82");
        test_invalid_utf8("\xE1\x20\x80\xE2\x30\x81\xE3\x40\x82");
        test_invalid_utf8("\xE1\x80\xE2\x81\xE3\x82");
    }

    TEST(invalid_utf8, four_bytes) {
        test_invalid_utf8("\xF0\x80\x80\x80\xF0\x81\x81\x81\xF0\x82\x82\x82");
        test_invalid_utf8("\xF1\x20\x80\x80\xF2\x30\x81\x81\xF3\x40\x82\x82");
        test_invalid_utf8("\xF1\x80\x80\xF2\x81\x81\xF3\x82\x82");
    }
} // namespace mjx

#ifdef _MJX_MSVC
#pragma warning(pop)
#endif // _MJX_MSVC