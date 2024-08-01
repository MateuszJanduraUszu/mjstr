// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjstr/string.hpp>

namespace mjx {
    TEST(string, assign) {
        utf8_string _Str;
        
        // assign a char 4 times
        _Str.assign(4, '=');
        EXPECT_EQ(_Str, "====");

        // assign a string
        string _Temp = "Exemplary";
        _Str.assign(_Temp);
        EXPECT_EQ(_Str, _Temp);

        // assign an r-value string
        _Str.assign(utf8_string{"C++ by "} + "example");
        EXPECT_EQ(_Str, "C++ by example");

        // assign a C-style string
        const char* const _Cstr = "C-style string";
        _Str.assign(_Cstr);
        EXPECT_EQ(_Str, _Cstr);
    }

    TEST(string, append) {
        const utf8_string& _Str = "string";
        const char* const _Ptr  = "C-string";
        utf8_string _Whole_str;

        // append a char 3 times 
        _Whole_str.append(3, '*');
        EXPECT_EQ(_Whole_str, "***");
 
        // append a whole string
        _Whole_str.push_back(' ');
        _Whole_str.append(_Str);
        EXPECT_EQ(_Whole_str, "*** string");

        // append a whole C-string
        _Whole_str.push_back(' ');
        _Whole_str.append(_Ptr);
        EXPECT_EQ(_Whole_str, "*** string C-string");
    }

    TEST(string, reserve) {
        utf8_string _Str;

        // test initial capacity
        EXPECT_EQ(_Str.capacity(), 15); // SSO active, small string capacity is 16 bytes (with null-terminator)
            
        // test capacity for 10 characters
        _Str.reserve(10);
        EXPECT_EQ(_Str.capacity(), 15); // requested less capacity than current, nothing has changed
        
        // test capacity for 100 characters
        _Str.reserve(100);
        constexpr size_t _Align        = 2 * sizeof(void*);
        constexpr size_t _Mask         = _Align - 1;
        constexpr size_t _Aligned_size = (100 + _Mask) & ~_Mask;
        EXPECT_EQ(_Str.capacity(), _Aligned_size - 1);
    }

    TEST(string, copy) {
        const utf8_string _Str = "ABCDEF";
        char _Buf[4]           = {'\0'};
    
        // copy the first three characters, _Buf should store "ABC"
        _Str.copy(_Buf, 3);
        EXPECT_STREQ(_Buf, "ABC");

        // copy three characters starting from the second one, _Buf should store "BCD"
        _Str.copy(_Buf, 3, 1);
        EXPECT_STREQ(_Buf, "BCD");

        // copy three characters starting from the third one, _Buf should store "CDE"
        _Str.copy(_Buf, 3, 2);
        EXPECT_STREQ(_Buf, "CDE");
    }

    TEST(string, swap) {
        utf8_string _Left  = "Left String";
        utf8_string _Right = "Right String";
        _Left.swap(_Right); // swap _Left's data with _Right's
        EXPECT_EQ(_Left, "Right String");
        EXPECT_EQ(_Right, "Left String");
    }

    TEST(string, clear) {
        utf8_string _Str     = "Exemplar";
        const auto _Capacity = _Str.capacity();
        _Str.clear();
        EXPECT_TRUE(_Str.empty());
        EXPECT_EQ(_Str.capacity(), _Capacity); // clear() doesn't change capacity
    }

    TEST(string, resize) {
        utf8_string _Str(20, 'x');

        // increase string size
        _Str.resize(30, 'y');
        EXPECT_EQ(_Str.size(), 30);
        EXPECT_EQ(_Str, utf8_string(20, 'x') + utf8_string(10, 'y'));
        
        // decrease string size
        _Str.resize(10);
        EXPECT_EQ(_Str.size(), 10);
        EXPECT_EQ(_Str, utf8_string(10, 'x'));
    }

    TEST(string, expand) {
        utf8_string _Str = "utf8_string_";
        _Str.expand(3, 'X');
        EXPECT_EQ(_Str, "utf8_string_XXX");
    }

    TEST(string, shrink) {
        utf8_string _Str = "utf8_string_view";

        // erase five last characters, the new string should be "utf8_string"
        _Str.shrink(5);
        EXPECT_EQ(_Str, "utf8_string");

        // erase seven last characters, the new string should be "utf8"
        _Str.shrink(7);
        EXPECT_EQ(_Str, "utf8");
    }

    TEST(string, erase) {
        utf8_string _Str = "This Is An Example";

        // erase " An", the new string should be "This Is Example"
        _Str.erase(7, 3);
        EXPECT_EQ(_Str, "This Is Example");

        // erase first " ", the new string should be "ThisIs Example"
        _Str.erase(_Str.begin() + _Str.find(' '));
        EXPECT_EQ(_Str, "ThisIs Example");

        // trim from " " to the end of the string, the new string should be "ThisIs"
        _Str.erase(_Str.find(' '));
        EXPECT_EQ(_Str, "ThisIs");

        // erase "Is", the new string should be "This"
        const utf8_string_const_iterator _First = _Str.begin() + _Str.find('s');
        _Str.erase(_First, _First + 2);
        EXPECT_EQ(_Str, "This");
    }

    TEST(string, insert) {
        utf8_string _Str = "xmplr";

        // insert "E" at the beginning
        _Str.insert(0, 1, 'E');
        EXPECT_EQ(_Str, "Exmplr");

        // insert "e" at the second place
        _Str.insert(2, "e");
        EXPECT_EQ(_Str, "Exemplr");

        // insert "a" at the sixth place
        _Str.insert(6, utf8_string{"a"});
        EXPECT_EQ(_Str, "Exemplar");
            
        // insert " is an example." at the end
        _Str.insert(8, " is an example.");
        EXPECT_EQ(_Str, "Exemplar is an example.");
        
        // insert ":" after "Exemplar"
        _Str.insert(_Str.begin() + 14, ':');
        EXPECT_EQ(_Str, "Exemplar is an: example.");
    }

    TEST(string, replace) {
        utf8_string _Str = "Hello World!";

        // replace "Hello" with "Hi"
        _Str.replace(0, 5, "Hi");
        EXPECT_EQ(_Str, "Hi World!");

        { // replace "World" with "John"
            const auto& _Iter = _Str.begin() + _Str.find('W');
            _Str.replace(_Iter, _Iter + 5, "John");
            EXPECT_EQ(_Str, "Hi John!");
        }

        { // replace "!" with "..."
            const auto& _Iter = _Str.begin() + _Str.size() - 1;
            _Str.replace(_Iter, _Iter + 1, 3, '.');
            EXPECT_EQ(_Str, "Hi John...");
        }
    }
} // namespace mjx