// string_iterator.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_STRING_ITERATOR_HPP_
#define _MJSTR_STRING_ITERATOR_HPP_
#include <cstddef>
#include <mjstr/api.hpp>

namespace mjx {
    template <class _Elem>
    class _MJSTR_API basic_string_view_iterator { // random access iterator for basic_string_view<T>
    public:
        using value_type      = _Elem;
        using difference_type = ptrdiff_t;
        using pointer         = const _Elem*;
        using reference       = const _Elem&;

        basic_string_view_iterator() noexcept;
#ifdef _DEBUG
        explicit basic_string_view_iterator(pointer _First, pointer _Last) noexcept;
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        basic_string_view_iterator(pointer _Ptr) noexcept;
#endif // _DEBUG

        // returns the first character of current substring
        reference operator*() const noexcept;

        // returns the current substring
        pointer operator->() const noexcept;

        // returns the element at specified offset
        reference operator[](const difference_type _Off) const noexcept;

        // advances the iterator to the next element
        basic_string_view_iterator& operator++() noexcept;

        // advances the iterator to the next element (performs post-incrementation)
        basic_string_view_iterator operator++(int) noexcept;

        // retreats the iterator to the previous element
        basic_string_view_iterator& operator--() noexcept;
    
        // retreats the iterator to the previous element (performs post-decrementation)
        basic_string_view_iterator operator--(int) noexcept;

        // advances the iterator by _Off elements
        basic_string_view_iterator& operator+=(const difference_type _Off) noexcept;

        // retreats the iterator by _Off elements
        basic_string_view_iterator& operator-=(const difference_type _Off) noexcept;

        // returns a new iterator that is _Off elements ahead the current one
        basic_string_view_iterator operator+(const difference_type _Off) const noexcept;

        // returns a new iterator that is _Off elements behind the current one
        basic_string_view_iterator operator-(const difference_type _Off) const noexcept;

        // checks if two iterators are equal
        bool operator==(const basic_string_view_iterator& _Other) const noexcept;

        // checks if two iterators are not equal
        bool operator!=(const basic_string_view_iterator& _Other) const noexcept;

        // checks if the current iterator is greater than other one
        bool operator>(const basic_string_view_iterator& _Other) const noexcept;

        // checks if the current iterator is greater than or equal to other one
        bool operator>=(const basic_string_view_iterator& _Other) const noexcept;
        
        // checks if the current iterator is less than other one
        bool operator<(const basic_string_view_iterator& _Other) const noexcept;

        // checks if the current iterator is less than or equal to other one
        bool operator<=(const basic_string_view_iterator& _Other) const noexcept;

    private:
        const _Elem* _Myptr;
#ifdef _DEBUG
        const _Elem* const _Mybegin;
        const _Elem* const _Myend;
#endif // _DEBUG
    };
} // namespace mjx

#endif // _MJSTR_STRING_ITERATOR_HPP_