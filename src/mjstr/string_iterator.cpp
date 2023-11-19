// string_iterator.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <mjstr/impl/utils.hpp>
#include <mjstr/string.hpp>
#include <mjstr/string_iterator.hpp>

namespace mjx {
    template <class _Elem>
    basic_string_view_iterator<_Elem>::basic_string_view_iterator() noexcept
#ifdef _DEBUG
        : _Myptr(nullptr), _Mybegin(nullptr), _Myend(nullptr) {}
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        : _Myptr(nullptr) {}
#endif // _DEBUG

    template <class _Elem>
    basic_string_view_iterator<_Elem>::basic_string_view_iterator(
#ifdef _DEBUG
        pointer _First, pointer _Last) noexcept : _Myptr(_First), _Mybegin(_First), _Myend(_Last) {
        _INTERNAL_ASSERT(_First <= _Last, "invalid iterator bounds");
    }
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        pointer _Ptr) noexcept : _Myptr(_Ptr) {}
#endif // _DEBUG

    template <class _Elem>
    typename basic_string_view_iterator<_Elem>::reference
        basic_string_view_iterator<_Elem>::operator*() const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr && _Myptr != _Myend, "attempt to dereference invalid iterator");
#endif // _DEBUG
        return *_Myptr;
    }

    template <class _Elem>
    typename basic_string_view_iterator<_Elem>::pointer
        basic_string_view_iterator<_Elem>::operator->() const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr && _Myptr != _Myend, "attempt to dereference invalid iterator");
#endif // _DEBUG
        return _Myptr;
    }

    template <class _Elem>
    typename basic_string_view_iterator<_Elem>::reference
        basic_string_view_iterator<_Elem>::operator[](const difference_type _Off) const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myend - _Myptr >= _Off, "attempt to access non-existent element");
#endif // _DEBUG
        return _Myptr[_Off];
    }

    template <class _Elem>
    basic_string_view_iterator<_Elem>& basic_string_view_iterator<_Elem>::operator++() noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myend - _Myptr > 0, "attempt to advance iterator that points to the end")
#endif // _DEBUG
        ++_Myptr;
        return *this;
    }

    template <class _Elem>
    basic_string_view_iterator<_Elem> basic_string_view_iterator<_Elem>::operator++(int) noexcept {
        const basic_string_view_iterator _Temp = *this;
        ++*this;
        return _Temp;
    }

    template <class _Elem>
    basic_string_view_iterator<_Elem>& basic_string_view_iterator<_Elem>::operator--() noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myptr - _Mybegin > 0, "attempt to retreat iterator that points to the beginning");
#endif // _DEBUG
        --_Myptr;
        return *this;
    }

    template <class _Elem>
    basic_string_view_iterator<_Elem> basic_string_view_iterator<_Elem>::operator--(int) noexcept {
        const basic_string_view_iterator _Temp = *this;
        --*this;
        return _Temp;
    }

    template <class _Elem>
    basic_string_view_iterator<_Elem>&
        basic_string_view_iterator<_Elem>::operator+=(const difference_type _Off) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myend - _Myptr >= _Off, "attempt to advance iterator beyond the end");
#endif // _DEBUG
        _Myptr += _Off;
        return *this;
    }

    template <class _Elem>
    basic_string_view_iterator<_Elem>&
        basic_string_view_iterator<_Elem>::operator-=(const difference_type _Off) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myptr - _Mybegin >= _Off, "attempt to retreat iterator beyond the beginning");
#endif // _DEBUG
        _Myptr -= _Off;
        return *this;
    }

    template <class _Elem>
    basic_string_view_iterator<_Elem>
        basic_string_view_iterator<_Elem>::operator+(const difference_type _Off) const noexcept {
        basic_string_view_iterator _Temp = *this;
        _Temp                           += _Off;
        return _Temp;
    }

    template <class _Elem>
    basic_string_view_iterator<_Elem>
        basic_string_view_iterator<_Elem>::operator-(const difference_type _Off) const noexcept {
        basic_string_view_iterator _Temp = *this;
        _Temp                           -= _Off;
        return _Temp;
    }

    template <class _Elem>
    bool basic_string_view_iterator<_Elem>::operator==(
        const basic_string_view_iterator& _Other) const noexcept {
        return _Myptr == _Other._Myptr;
    }

    template <class _Elem>
    bool basic_string_view_iterator<_Elem>::operator!=(
        const basic_string_view_iterator& _Other) const noexcept {
        return _Myptr != _Other._Myptr;
    }

    template <class _Elem>
    bool basic_string_view_iterator<_Elem>::operator>(
        const basic_string_view_iterator& _Other) const noexcept {
        return _Myptr > _Other._Myptr;
    }

    template <class _Elem>
    bool basic_string_view_iterator<_Elem>::operator>=(
        const basic_string_view_iterator& _Other) const noexcept {
        return _Myptr >= _Other._Myptr;
    }

    template <class _Elem>
    bool basic_string_view_iterator<_Elem>::operator<(
        const basic_string_view_iterator& _Other) const noexcept {
        return _Myptr < _Other._Myptr;
    }

    template <class _Elem>
    bool basic_string_view_iterator<_Elem>::operator<=(
        const basic_string_view_iterator& _Other) const noexcept {
        return _Myptr <= _Other._Myptr;
    }

    template _MJSTR_API class basic_string_view_iterator<byte_t>;
    template _MJSTR_API class basic_string_view_iterator<char>;
    template _MJSTR_API class basic_string_view_iterator<wchar_t>;
} // namespace mjx