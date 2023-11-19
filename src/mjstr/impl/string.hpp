// string.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_IMPL_STRING_HPP_
#define _MJSTR_IMPL_STRING_HPP_
#include <cstring>
#include <cwchar>
#include <mjstr/impl/utils.hpp>
#include <type_traits>

namespace mjx {
    namespace mjstr_impl {
        template <class _Elem, class _Comp_fn>
        constexpr size_t _Common_find_n(const _Elem* const _Haystack, const size_t _Haystack_size,
            const _Elem* const _Needle, const size_t _Needle_size, _Comp_fn _Comp) noexcept {
            const _Elem* _First      = _Haystack;
            const _Elem* const _Last = _First + _Haystack_size;
            for (; _First != _Last; ++_First) {
                if (*_First == *_Needle) {
                    if (_Comp(_First + 1, _Needle + 1, _Needle_size - 1)) {
                        return _First - _Haystack;
                    }
                }
            }

            return static_cast<size_t>(-1); // not found
        }

        template <class _Elem>
        constexpr size_t _Common_rfind_ch(
            const _Elem* const _Haystack, const size_t _Begin, const _Elem _Needle) noexcept {
            const _Elem* _Last = _Haystack + _Begin;
            for (; _Last >= _Haystack; --_Last) { // iterate from end to beginning
                if (*_Last == _Needle) {
                    return _Last - _Haystack;
                }
            }

            return static_cast<size_t>(-1); // not found
        }

        template <class _Elem, class _Comp_fn>
        constexpr size_t _Common_rfind_str(const _Elem* const _Haystack, const size_t _Begin,
            const _Elem* const _Needle, const size_t _Needle_size, _Comp_fn _Comp) noexcept {
            const _Elem* _Last = _Haystack + _Begin;
            for (; _Last >= _Haystack; --_Last) {
                if (*_Last == *_Needle) {
                    if (_Comp(_Last + 1, _Needle + 1, _Needle_size - 1)) {
                        return _Last - _Haystack;
                    }
                }
            }

            return static_cast<size_t>(-1); // not found
        }

#pragma intrinsic(strlen) // UCRT's strlen() intrinsic
#pragma intrinsic(wcslen) // UCRT's wcslen() intrinsic

        struct _Utf8_string_traits { // traits for UTF-8 strings
            using _Char_type = char;

            static size_t _Length(const _Char_type* const _Str) noexcept {
#if _HAS_BUILTIN(__builtin_strlen)
                return __builtin_strlen(_Str);
#else // ^^^ _HAS_BUILTIN(__builtin_strlen) ^^^ / vvv !_HAS_BUILTIN(__builtin_strlen) vvv
                return ::strlen(_Str);
#endif // _HAS_BUILTIN(__builtin_strlen)
            }
            
            static bool _Equal(
                const _Char_type* const _Left, const _Char_type* const _Right, const size_t _Count) noexcept {
#if _HAS_BUILTIN(__builtin_strncmp)
                return __builtin_strncmp(_Left, _Right, _Count) == 0;
#else // ^^^ _HAS_BUILTIN(__builtin_strncmp) ^^^ / vvv !_HAS_BUILTIN(__builtin_strncmp) vvv
                return ::strncmp(_Left, _Right, _Count) == 0;
#endif // _HAS_BUILTIN(__builtin_strncmp)
            }

            static int _Compare(const _Char_type* const _Left, const size_t _Left_size,
                const _Char_type* const _Right, const size_t _Right_size) noexcept {
                if (_Left_size != _Right_size) {
                    return _Left_size < _Right_size ? -1 : 1;
                }

#if _HAS_BUILTIN(__builtin_strncmp)
                return __builtin_strncmp(_Left, _Right, _Left_size);
#else // ^^^ _HAS_BUILTIN(__builtin_strncmp) ^^^ / vvv !_HAS_BUILTIN(__builtin_strncmp) vvv
                return ::strncmp(_Left, _Right, _Left_size);
#endif // _HAS_BUILTIN(__builtin_strncmp)
            }

            static void _Copy(
                _Char_type* const _Dest, const _Char_type* const _Src, const size_t _Count) noexcept {
#pragma warning(suppress : 4996) // C4996: Consider using strncpy_s()
                ::strncpy(_Dest, _Src, _Count);
            }

            static size_t _Find(const _Char_type* const _Haystack, const _Char_type _Needle) noexcept {
#if _HAS_BUILTIN(__builtin_strchr)
                const _Char_type* const _Match = __builtin_strchr(_Haystack, static_cast<int>(_Needle));
#else // ^^^ _HAS_BUILTIN(__builtin_strchr) ^^^ / vvv !_HAS_BUILTIN(__builtin_strchr) vvv
                const _Char_type* const _Match = ::strchr(_Haystack, static_cast<int>(_Needle));
#endif // _HAS_BUILTIN(__builtin_strchr)
                return _Match ? _Match - _Haystack : static_cast<size_t>(-1);
            }

            static size_t _Find(const _Char_type* const _Haystack, const _Char_type* const _Needle) noexcept {
                const _Char_type* const _Match = ::strstr(_Haystack, _Needle);
                return _Match ? _Match - _Haystack : static_cast<size_t>(-1);
            }

            static size_t _Find_n(const _Char_type* const _Haystack, const size_t _Haystack_size,
                const _Char_type* const _Needle, const size_t _Needle_size) noexcept {
                return _Common_find_n(_Haystack, _Haystack_size, _Needle, _Needle_size, &_Equal);
            }

            static size_t _Rfind(const _Char_type* const _Haystack,
                const size_t _Haystack_size, const _Char_type _Needle) noexcept {
                return _Common_rfind_ch(_Haystack, _Haystack_size, _Needle);
            }

            static size_t _Rfind(const _Char_type* const _Haystack, const size_t _Haystack_size,
                const _Char_type* const _Needle, const size_t _Needle_size) noexcept {
                return _Common_rfind_str(_Haystack, _Haystack_size, _Needle, _Needle_size, &_Equal);
            }
        };

        struct _Byte_string_traits { // traits for byte strings
            // Note: Since "unsigned char" has the same size as "char", we can safely cast
            //       "unsigned char" to "char" for compatibility without any loss of information.
            using _Base_traits    = _Utf8_string_traits;
            using _Base_char_type = _Base_traits::_Char_type;

            using _Char_type = unsigned char;
            
            static size_t _Length(const _Char_type* const _Str) noexcept {
                return _Base_traits::_Length(reinterpret_cast<const _Base_char_type*>(_Str));
            }

            static size_t _Equal(
                const _Char_type* const _Left, const _Char_type* const _Right, const size_t _Count) noexcept {
                return _Base_traits::_Equal(reinterpret_cast<const _Base_char_type*>(_Left),
                    reinterpret_cast<const _Base_char_type*>(_Right), _Count);
            }

            static int _Compare(const _Char_type* const _Left, const size_t _Left_size,
                const _Char_type* const _Right, const size_t _Right_size) noexcept {
                return _Base_traits::_Compare(reinterpret_cast<const _Base_char_type*>(_Left),
                    _Left_size, reinterpret_cast<const _Base_char_type*>(_Right), _Right_size);
            }

            static void _Copy(
                _Char_type* const _Dest, const _Char_type* const _Src, const size_t _Count) noexcept {
                _Base_traits::_Copy(reinterpret_cast<_Base_char_type*>(_Dest),
                    reinterpret_cast<const _Base_char_type*>(_Src), _Count);
            }

            static size_t _Find(const _Char_type* const _Haystack, const _Char_type _Needle) noexcept {
                return _Base_traits::_Find(reinterpret_cast<const _Base_char_type*>(_Haystack),
                    static_cast<_Base_char_type>(_Needle));
            }

            static size_t _Find(const _Char_type* const _Haystack, const _Char_type* const _Needle) noexcept {
                return _Base_traits::_Find(reinterpret_cast<const _Base_char_type*>(_Haystack),
                    reinterpret_cast<const _Base_char_type*>(_Needle));
            }

            static size_t _Find_n(const _Char_type* const _Haystack, const size_t _Haystack_size,
                const _Char_type* const _Needle, const size_t _Needle_size) noexcept {
                return _Common_find_n(_Haystack, _Haystack_size, _Needle, _Needle_size, &_Equal);
            }

            static size_t _Rfind(const _Char_type* const _Haystack,
                const size_t _Haystack_size, const _Char_type _Needle) noexcept {
                return _Common_rfind_ch(_Haystack, _Haystack_size, _Needle);
            }

            static size_t _Rfind(const _Char_type* const _Haystack, const size_t _Begin,
                const _Char_type* const _Needle, const size_t _Needle_size) noexcept {
                return _Common_rfind_str(_Haystack, _Begin, _Needle, _Needle_size, &_Equal);
            }
        };

        struct _Unicode_string_traits { // traits for Unicode strings
            using _Char_type = wchar_t;

            static size_t _Length(const _Char_type* const _Str) noexcept {
#if _HAS_BUILTIN(__builtin_wcslen)
                return __builtin_wcslen(_Str);
#else // ^^^ _HAS_BUILTIN(__builtin_wcslen) ^^^ / vvv !_HAS_BUILTIN(__builtin_wcslen) vvv
                return ::wcslen(_Str);
#endif // _HAS_BUILTIN(__builtin_wcslen)
            }

            static bool _Equal(
                const _Char_type* const _Left, const _Char_type* const _Right, const size_t _Count) noexcept {
#if _HAS_BUILTIN(__builtin_wcsncmp)
                return __builtin_wcsncmp(_Left, _Right, _Count) == 0;
#else // ^^^ _HAS_BUILTIN(__builtin_wcsncmp) ^^^ / vvv !_HAS_BUILTIN(__builtin_wcsncmp) vvv
                return ::wcsncmp(_Left, _Right, _Count) == 0;
#endif // _HAS_BUILTIN(__builtin_wcsncmp)
            }

            static int _Compare(const _Char_type* const _Left, const size_t _Left_size,
                const _Char_type* const _Right, const size_t _Right_size) noexcept {
                if (_Left_size != _Right_size) {
                    return _Left_size < _Right_size ? -1 : 1;
                }

#if _HAS_BUILTIN(__builtin_wcsncmp)
                return __builtin_wcsncmp(_Left, _Right, _Left_size);
#else // ^^^ _HAS_BUILTIN(__builtin_wcsncmp) ^^^ / vvv !_HAS_BUILTIN(__builtin_wcsncmp) vvv
                return ::wcsncmp(_Left, _Right, _Left_size);
#endif // _HAS_BUILTIN(__builtin_wcsncmp)
            }

            static void _Copy(
                _Char_type* const _Dest, const _Char_type* const _Src, const size_t _Count) noexcept {
#pragma warning(suppress : 4996) // C4996: Consider using wcsncpy_s()
                ::wcsncpy(_Dest, _Src, _Count);
            }

            static size_t _Find(const _Char_type* const _Haystack, const _Char_type _Needle) noexcept {
#if _HAS_BUILTIN(__builtin_wcschr)
                const _Char_type* const _Match = __builtin_wcschr(_Haystack, static_cast<int>(_Needle));
#else // ^^^ _HAS_BUILTIN(__builtin_wcschr) ^^^ / vvv !_HAS_BUILTIN(__builtin_wcschr) vvv
                const _Char_type* const _Match = ::wcschr(_Haystack, static_cast<int>(_Needle));
#endif // _HAS_BUILTIN(__builtin_wcschr)
                return _Match ? _Match - _Haystack : static_cast<size_t>(-1);
            }

            static size_t _Find(const _Char_type* const _Haystack, const _Char_type* const _Needle) noexcept {
                const _Char_type* const _Match = ::wcsstr(_Haystack, _Needle);
                return _Match ? _Match - _Haystack : static_cast<size_t>(-1);
            }

            static size_t _Find_n(const _Char_type* const _Haystack, const size_t _Haystack_size,
                const _Char_type* const _Needle, const size_t _Needle_size) noexcept {
                return _Common_find_n(_Haystack, _Haystack_size, _Needle, _Needle_size, &_Equal);
            }

            static size_t _Rfind(const _Char_type* const _Haystack,
                const size_t _Haystack_size, const _Char_type _Needle) noexcept {
                return _Common_rfind_ch(_Haystack, _Haystack_size, _Needle);
            }

            static size_t _Rfind(const _Char_type* const _Haystack, const size_t _Haystack_size,
                const _Char_type* const _Needle, const size_t _Needle_size) noexcept {
                return _Common_rfind_str(_Haystack, _Haystack_size, _Needle, _Needle_size, &_Equal);
            }
        };

        template <class _Elem>
        struct _Choose_string_traits {};

        template <>
        struct _Choose_string_traits<unsigned char> {
            using _Traits = _Byte_string_traits;
        };

        template <>
        struct _Choose_string_traits<char> {
            using _Traits = _Utf8_string_traits;
        };

        template <>
        struct _Choose_string_traits<wchar_t> {
            using _Traits = _Unicode_string_traits;
        };

        template <class _Elem>
        using _String_traits = typename _Choose_string_traits<_Elem>::_Traits;
    } // namespace mjstr_impl
} // namespace mjx

#endif // _MJSTR_IMPL_STRING_HPP_