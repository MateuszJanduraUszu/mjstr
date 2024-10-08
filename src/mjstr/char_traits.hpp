// char_traits.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_CHAR_TRAITS_HPP_
#define _MJSTR_CHAR_TRAITS_HPP_
#include <compare>
#include <cstddef>
#include <mjstr/api.hpp>
#include <type_traits>

namespace mjx {
    using byte_t = unsigned char; // byte representation (1-byte non-negative integer)

    template <class _Elem>
    concept compatible_element = ::std::disjunction_v<
        ::std::is_same<_Elem, byte_t>, ::std::is_same<_Elem, char>, ::std::is_same<_Elem, wchar_t>>;

    template <class _Elem>
    struct _MJSTR_API char_traits {
        static_assert(compatible_element<_Elem>, "invalid element type for char_traits<CharT>");

        using char_type           = _Elem;
        using comparison_category = ::std::strong_ordering;

        // assigns characters
        static void assign(char_type* _Dest, const size_t _Count, const char_type _Ch) noexcept;

        // checks whether two character sequence are equal
        static bool eq(
            const char_type* const _Left, const char_type* const _Right, const size_t _Count) noexcept;

        // moves one character sequence onto another
        static void move(char_type* const _Dest, const char_type* const _Src, const size_t _Count) noexcept;

        // copies a character sequence
        static void copy(char_type* const _Dest, const char_type* const _Src, const size_t _Count) noexcept;

        // lexicographically compares two character sequence
        static int compare(
            const char_type* const _Left, const char_type* const _Right, const size_t _Count) noexcept;

        // returns the length of a character sequence
        static size_t length(const char_type* const _Str) noexcept;

        // finds characters in a character sequence
        static size_t find(
            const char_type* const _Haystack, const size_t _Haystack_size, const char_type _Needle) noexcept;
        static size_t find(const char_type* const _Haystack, const size_t _Haystack_size,
            const char_type* const _Needle, const size_t _Needle_size) noexcept;

        // finds characters in a character sequence, searchs from the end
        static size_t rfind(
            const char_type* const _Haystack,const size_t _Haystack_size, const char_type _Needle) noexcept;
        static size_t rfind(const char_type* const _Haystack, const size_t _Haystack_size,
            const char_type* const _Needle, const size_t _Needle_size) noexcept;
    };
} // namespace mjx

#endif // _MJSTR_CHAR_TRAITS_HPP_