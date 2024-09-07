// char_traits.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <mjstr/char_traits.hpp>
#include <mjstr/impl/char_traits.hpp>

namespace mjx {
    template <class _Elem>
    void char_traits<_Elem>::assign(char_type* const _Dest, const size_t _Count, const char_type _Ch) noexcept {
        mjstr_impl::_Char_traits<_Elem>::_Assign(_Dest, _Count, _Ch);
    }

    template <class _Elem>
    bool char_traits<_Elem>::eq(
        const char_type* const _Left, const char_type* const _Right, const size_t _Count) noexcept {
        return mjstr_impl::_Char_traits<_Elem>::_Eq(_Left, _Right, _Count);
    }

    template <class _Elem>
    void char_traits<_Elem>::move(
        char_type* const _Dest, const char_type* const _Src, const size_t _Count) noexcept {
        mjstr_impl::_Char_traits<_Elem>::_Move(_Dest, _Src, _Count);
    }

    template <class _Elem>
    void char_traits<_Elem>::copy(
        char_type* const _Dest, const char_type* const _Src, const size_t _Count) noexcept {
        mjstr_impl::_Char_traits<_Elem>::_Copy(_Dest, _Src, _Count);
    }

    template <class _Elem>
    int char_traits<_Elem>::compare(
        const char_type* const _Left, const char_type* const _Right, const size_t _Count) noexcept {
        return mjstr_impl::_Char_traits<_Elem>::_Compare(_Left, _Right, _Count);
    }

    template <class _Elem>
    size_t char_traits<_Elem>::length(const char_type* const _Str) noexcept {
        return mjstr_impl::_Char_traits<_Elem>::_Length(_Str);
    }

    template <class _Elem>
    size_t char_traits<_Elem>::find(
        const char_type* const _Haystack, const size_t _Haystack_size, const char_type _Needle) noexcept {
        return mjstr_impl::_Char_traits<_Elem>::_Find(_Haystack, _Haystack_size, _Needle);
    }

    template <class _Elem>
    size_t char_traits<_Elem>::find(const char_type* const _Haystack, const size_t _Haystack_size,
        const char_type* const _Needle, const size_t _Needle_size) noexcept {
        const char_type* _First      = _Haystack;
        const char_type* const _Last = _First + _Haystack_size;
        for (; _First != _Last; ++_First) {
            if (*_First == *_Needle) {
                if (eq(_First + 1, _Needle + 1, _Needle_size - 1)) {
                    return _First - _Haystack;
                }
            }
        }

        return static_cast<size_t>(-1); // not found
    }

    template <class _Elem>
    size_t char_traits<_Elem>::rfind(
        const char_type* const _Haystack, const size_t _Haystack_size, const char_type _Needle) noexcept {
        const char_type* _Last = _Haystack + _Haystack_size;
        for (; _Last >= _Haystack; --_Last) { // iterate from end to beginning
            if (*_Last == _Needle) {
                return _Last - _Haystack;
            }
        }

        return static_cast<size_t>(-1); // not found
    }

    template <class _Elem>
    size_t char_traits<_Elem>::rfind(const char_type* const _Haystack, const size_t _Haystack_size,
        const char_type* const _Needle, const size_t _Needle_size) noexcept {
        const char_type* _Last = _Haystack + _Haystack_size;
        for (; _Last >= _Haystack; --_Last) { // iterate from end to beginning
            if (*_Last == *_Needle) {
                if (eq(_Last + 1, _Needle + 1, _Needle_size - 1)) {
                    return _Last - _Haystack;
                }
            }
        }

        return static_cast<size_t>(-1); // not found
    }

    template struct _MJSTR_API char_traits<byte_t>;
    template struct _MJSTR_API char_traits<char>;
    template struct _MJSTR_API char_traits<wchar_t>;
} // namespace mjx