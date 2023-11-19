// string.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_STRING_HPP_
#define _MJSTR_STRING_HPP_
#include <cstddef>
#include <mjstr/api.hpp>
#include <mjstr/string_iterator.hpp>
#include <type_traits>

namespace mjx {
    using byte_t = unsigned char; // byte representation (1-byte non-negative integer)

    template <class _Ty, class... _Types>
    inline constexpr bool _Is_any_of = ::std::disjunction_v<::std::is_same<_Ty, _Types>...>;

    template <class _Elem>
    inline constexpr bool _Is_valid_string_element = _Is_any_of<_Elem, byte_t, char, wchar_t>;

    template <class _Elem>
    class _MJSTR_API basic_string_view { // lightweight non-owning read-only view of a string
    public:
        static_assert(_Is_valid_string_element<_Elem>, "invalid basic_string_view element type");

        using value_type      = _Elem;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;
        using pointer         = _Elem*;
        using const_pointer   = const _Elem*;
        using reference       = _Elem&;
        using const_reference = const _Elem&;
        
        using const_iterator = basic_string_view_iterator<_Elem>;
        using iterator       = const_iterator;

        static constexpr size_type npos = static_cast<size_type>(-1);

        basic_string_view() noexcept;
        basic_string_view(const_pointer _Ptr, const size_type _Count) noexcept;
        basic_string_view(const_pointer _Ptr) noexcept;

        basic_string_view(const basic_string_view&) noexcept            = default;
        basic_string_view& operator=(const basic_string_view&) noexcept = default;

        basic_string_view(nullptr_t) = delete;

        // accesses the specified character
        const_reference operator[](const size_type _Idx) const noexcept;

        // returns an iterator to the beginning
        const_iterator begin() const noexcept;

        // returns an iterator to the end
        const_iterator end() const noexcept;

        // accesses the specified character with bounds checking
        const_reference at(const size_type _Idx) const noexcept;

        // accesses the first character
        const_reference front() const noexcept;

        // accesses the last character
        const_reference back() const noexcept;

        // returns a pointer to the first character of a view
        const_pointer data() const noexcept;

        // returns the number of characters
        size_type size() const noexcept;

        // checks whether the view is empty
        bool empty() const noexcept;

        // shrinks the view by moving its start forward
        bool remove_prefix(const size_type _Count) noexcept;

        // shrinks the view by moving its end backward
        bool remove_suffix(const size_type _Count) noexcept;

        // swaps the contents
        void swap(basic_string_view& _Other) noexcept;

        // copies characters
        size_type copy(pointer _Dest, size_type _Count, const size_type _Off = 0) const noexcept;

        // returns a substring
        basic_string_view substr(const size_type _Off = 0, size_type _Count = npos) const noexcept;

        // compares two views
        int compare(const basic_string_view _Str) const noexcept;
        int compare(
            const size_type _Off, const size_type _Count, const basic_string_view _Str) const noexcept;
        int compare(const size_type _Off, const size_type _Count, const basic_string_view _Str,
            const size_type _Str_off, const size_type _Str_count) const noexcept;
        int compare(const_pointer _Ptr) const noexcept;
        int compare(const size_type _Off, const size_type _Count, const_pointer _Ptr) const noexcept;
        int compare(const size_type _Off, const size_type _Count,
            const_pointer _Ptr, const size_type _Ptr_count) const noexcept;

        // checks if the string view starts with the given prefix
        bool starts_with(const basic_string_view _Str) const noexcept;
        bool starts_with(const value_type _Ch) const noexcept;
        bool starts_with(const_pointer _Ptr) const noexcept;

        // checks if the string view ends with the given suffix
        bool ends_with(const basic_string_view _Str) const noexcept;
        bool ends_with(const value_type _Ch) const noexcept;
        bool ends_with(const_pointer _Ptr) const noexcept;

        // checks if the string view contains the given substring or character
        bool contains(const basic_string_view _Str) const noexcept;
        bool contains(const value_type _Ch) const noexcept;
        bool contains(const_pointer _Ptr) const noexcept;

        // find characters in the view
        size_type find(const basic_string_view _Str, const size_type _Off = 0) const noexcept;
        size_type find(const value_type _Ch, const size_type _Off = 0) const noexcept;
        size_type find(
            const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept;
        size_type find(const_pointer _Ptr, const size_type _Off = 0) const noexcept;

        // finds the last occurrence of a substring
        size_type rfind(const basic_string_view _Str, const size_type _Off = npos) const noexcept;
        size_type rfind(const value_type _Ch, const size_type _Off = npos) const noexcept;
        size_type rfind(
            const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept;
        size_type rfind(const_pointer _Ptr, const size_type _Off = npos) const noexcept;

    private:
        const_pointer _Mydata;
        size_type _Mysize;
    };

    template <class _Elem>
    _MJSTR_API bool operator==(
        const basic_string_view<_Elem> _Left, const basic_string_view<_Elem> _Right) noexcept;

    template <class _Elem>
    _MJSTR_API bool operator==(const basic_string_view<_Elem> _Left, const _Elem* const _Right) noexcept;

    template <class _Elem>
    _MJSTR_API bool operator==(const _Elem* const _Left, const basic_string_view<_Elem> _Right) noexcept;

    template <class _Elem>
    _MJSTR_API bool operator!=(
        const basic_string_view<_Elem> _Left, const basic_string_view<_Elem> _Right) noexcept;

    template <class _Elem>
    _MJSTR_API bool operator!=(const basic_string_view<_Elem> _Left, const _Elem* const _Right) noexcept;

    template <class _Elem>
    _MJSTR_API bool operator!=(const _Elem* const _Left, const basic_string_view<_Elem> _Right) noexcept;

    using byte_string_view    = basic_string_view<byte_t>;
    using utf8_string_view    = basic_string_view<char>;
    using unicode_string_view = basic_string_view<wchar_t>;
} // namespace mjx

#endif // _MJSTR_STRING_HPP_