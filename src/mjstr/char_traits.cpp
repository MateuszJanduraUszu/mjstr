// char_traits.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <cstring>
#include <cwchar>
#include <mjstr/char_traits.hpp>
#include <mjstr/impl/utils.hpp>

// prefer using UCRT intrinsics
#pragma intrinsic(memcmp)
#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(strlen)
#pragma intrinsic(wcslen)

namespace mjx {
    template <class _Elem>
    void char_traits<_Elem>::assign(
        char_type* const _Dest, const size_t _Count, const char_type _Ch) noexcept {
        if constexpr (sizeof(_Elem) == 2) { // assign 2-byte character
            ::wmemset(_Dest, _Ch, _Count);
        } else { // assign 1-byte character
            ::memset(_Dest, _Ch, _Count);
        }
    }

    template <class _Elem>
    bool char_traits<_Elem>::eq(
        const char_type* const _Left, const char_type* const _Right, const size_t _Count) noexcept {
        if constexpr (sizeof(_Elem) == 2) { // check whether 2-byte characters are equal
#if _HAS_BUILTIN(__builtin_wmemcmp)
            return __builtin_wmemcmp(_Left, _Right, _Count) == 0;
#else // ^^^ _HAS_BUILTIN(__builtin_wmemcmp) ^^^ / vvv !_HAS_BUILTIN(__builtin_wmemcmp) vvv
            return ::wmemcmp(_Left, _Right, _Count) == 0;
#endif // _HAS_BUILTIN(__builtin_wmemcmp)
        } else { // check whether 1-byte characters are equal
#if _HAS_BUILTIN(__builtin_memcmp)
            return __builtin_memcmp(_Left, _Right, _Count) == 0;
#else // ^^^ _HAS_BUILTIN(__builtin_memcmp) ^^^ / vvv !_HAS_BUILTIN(__builtin_memcmp) vvv
            return ::memcmp(_Left, _Right, _Count) == 0;
#endif // _HAS_BUILTIN(__builtin_memcmp)
        }
    }

    template <class _Elem>
    void char_traits<_Elem>::move(
        char_type* const _Dest, const char_type* const _Src, const size_t _Count) noexcept {
        if constexpr (sizeof(_Elem) == 2) { // move 2-byte characters
            ::wmemmove(_Dest, _Src, _Count);
        } else { // move 1-byte characters
            ::memmove(_Dest, _Src, _Count);
        }
    }

    template <class _Elem>
    void char_traits<_Elem>::copy(
        char_type* const _Dest, const char_type* const _Src, const size_t _Count) noexcept {
        if constexpr (sizeof(_Elem) == 2) { // copy 2-byte characters
            ::wmemcpy(_Dest, _Src, _Count);
        } else { // copy 1-byte characters
            ::memcpy(_Dest, _Src, _Count);
        }
    }

    template <class _Elem>
    int char_traits<_Elem>::compare(const char_type* const _Left, const size_t _Left_size,
        const char_type* const _Right, const size_t _Right_size) noexcept {
        if (_Left_size != _Right_size) {
            return _Left_size < _Right_size ? -1 : 1;
        }

        if constexpr (sizeof(_Elem) == 2) { // compare 2-byte characters
#if _HAS_BUILTIN(__builtin_wmemcmp)
            return __builtin_wmemcmp(_Left, _Right, _Left_size);
#else // ^^^ _HAS_BUILTIN(__builtin_wmemcmp) ^^^ / vvv !_HAS_BUILTIN(__builtin_wmemcmp) vvv
            return ::wmemcmp(_Left, _Right, _Left_size);
#endif // _HAS_BUILTIN(__builtin_wmemcmp)
        } else { // compare 1-byte characters
#if _HAS_BUILTIN(__builtin_memcmp)
            return __builtin_memcmp(_Left, _Right, _Left_size);
#else // ^^^ _HAS_BUILTIN(__builtin_memcmp) ^^^ / vvv !_HAS_BUILTIN(__builtin_memcmp) vvv
            return ::memcmp(_Left, _Right, _Left_size);
#endif // _HAS_BUILTIN(__builtin_memcmp)
        }
    }

    template <class _Elem>
    size_t char_traits<_Elem>::length(const char_type* const _Str) noexcept {
        if constexpr (sizeof(_Elem) == 2) { // calculate length of a 2-byte character sequence
#if _HAS_BUILTIN(__builtin_wcslen)
            return __builtin_wcslen(_Str);
#else // ^^^ _HAS_BUILTIN(__builtin_wcslen) ^^^ / vvv !_HAS_BUILTIN(__builtin_wcslen) vvv
            return ::wcslen(_Str);
#endif // _HAS_BUILTIN(__builtin_wcslen)
        } else { // calculate length of a 1-byte character sequence
#if _HAS_BUILTIN(__builtin_strlen)
            return __builtin_strlen(reinterpret_cast<const char*>(_Str));
#else // ^^^ _HAS_BUILTIN(__builtin_strlen) ^^^ / vvv !_HAS_BUILTIN(__builtin_strlen) vvv
            return ::strlen(reinterpret_cast<const char*>(_Str));
#endif // _HAS_BUILTIN(__builtin_strlen)
        }
    }

    template <class _Elem>
    size_t char_traits<_Elem>::find(
        const char_type* const _Haystack, const size_t _Haystack_size, const char_type _Needle) noexcept {
        const char_type* _Match;
        if constexpr (sizeof(_Elem) == 2) { // search within 2-byte character sequence
#if _HAS_BUILTIN(__builtin_wmemchr)
            _Match = static_cast<const char_type*>(__builtin_wmemchr(_Haystack, _Needle, _Haystack_size));
#else // ^^^ _HAS_BUILTIN(__builtin_wmemchr) ^^^ / vvv !_HAS_BUILTIN(__builtin_wmemchr) vvv
            _Match = static_cast<const char_type*>(::wmemchr(_Haystack, _Needle, _Haystack_size));
#endif // _HAS_BUILTIN(__builtin_wmemchr)
        } else { // search within 1-byte character sequence
#if _HAS_BUILTIN(__builtin_memchr)
            _Match = static_cast<const char_type*>(__builtin_memchr(_Haystack, _Needle, _Haystack_size));
#else // ^^^ _HAS_BUILTIN(__builtin_memchr) ^^^ / vvv !_HAS_BUILTIN(__builtin_memchr) vvv
            _Match = static_cast<const char_type*>(::memchr(_Haystack, _Needle, _Haystack_size));
#endif // _HAS_BUILTIN(__builtin_memchr)
        }

        return _Match ? _Match - _Haystack : static_cast<size_t>(-1);
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

    template _MJSTR_API struct char_traits<byte_t>;
    template _MJSTR_API struct char_traits<char>;
    template _MJSTR_API struct char_traits<wchar_t>;
} // namespace mjx