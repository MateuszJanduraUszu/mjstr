// char_traits.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_IMPL_CHAR_TRAITS_HPP_
#define _MJSTR_IMPL_CHAR_TRAITS_HPP_
#include <cstring>
#include <cwchar>
#include <type_traits>

namespace mjx {
    namespace mjstr_impl {
        template <class _Elem>
        struct _Narrow_char_traits { // specialization of char_traits for char and byte_t types
            static void _Assign(_Elem* const _Dest, const size_t _Count, const _Elem _Ch) noexcept {
#if defined(_MJX_CLANG) || defined(_MJX_GCC)
                __builtin_memset(_Dest, _Ch, _Count);
#else // ^^^ Clang or GCC ^^^ / vvv MSVC vvv
                ::memset(_Dest, _Ch, _Count);
#endif // defined(_MJX_CLANG) || defined(_MJX_GCC)
            }

            static bool _Eq(const _Elem* const _Left, const _Elem* const _Right, const size_t _Count) noexcept {
                return __builtin_memcmp(_Left, _Right, _Count) == 0;
            }

            static void _Move(_Elem* const _Dest, const _Elem* const _Src, const size_t _Count) noexcept {
#if defined(_MJX_CLANG) || defined(_MJX_GCC)
                __builtin_memmove(_Dest, _Src, _Count);
#else // ^^^ Clang or GCC ^^^ / vvv MSVC vvv
                ::memmove(_Dest, _Src, _Count);
#endif // defined(_MJX_CLANG) || defined(_MJX_GCC)
            }

            static void _Copy(_Elem* const _Dest, const _Elem* const _Src, const size_t _Count) noexcept {
#if defined(_MJX_CLANG) || defined(_MJX_GCC)
                __builtin_memcpy(_Dest, _Src, _Count);
#else // ^^^ Clang or GCC ^^^ / vvv MSVC vvv
                ::memcpy(_Dest, _Src, _Count);
#endif // defined(_MJX_CLANG) || defined(_MJX_GCC)
            }

            static int _Compare(const _Elem* const _Left, const _Elem* const _Right, const size_t _Count) noexcept {
                return __builtin_memcmp(_Left, _Right, _Count);
            }

            static size_t _Length(const _Elem* const _Str) noexcept {
                return __builtin_strlen(reinterpret_cast<const char*>(_Str));
            }

            static size_t _Find(
                const _Elem* const _Haystack, const size_t _Haystack_size, const _Elem _Needle) noexcept {
                const _Elem* const _Match = reinterpret_cast<const _Elem*>(
#if defined(_MJX_CLANG) || defined(_MJX_MSVC)
                    __builtin_char_memchr(reinterpret_cast<const char*>(_Haystack), _Needle, _Haystack_size)
#else // ^^^ Clang or MSVC ^^^ / vvv GCC vvv
                    __builtin_memchr(_Haystack, _Needle, _Haystack_size)
#endif // defined(_MJX_CLANG) || defined(_MJX_MSVC)
                );
                return _Match ? _Match - _Haystack : static_cast<size_t>(-1);
            }
        };

        struct _Wide_char_traits { // specialization of char_traits for wchar_t type
            static void _Assign(wchar_t* const _Dest, const size_t _Count, const wchar_t _Ch) noexcept {
                ::wmemset(_Dest, _Ch, _Count);
            }

            static bool _Eq(const wchar_t* const _Left, const wchar_t* const _Right, const size_t _Count) noexcept {
#if defined(_MJX_CLANG) || defined(_MJX_MSVC)
                return __builtin_wmemcmp(_Left, _Right, _Count) == 0;
#else // ^^^ Clang or MSVC ^^^ / vvv GCC vvv
                return ::wmemcmp(_Left, _Right, _Count) == 0;
#endif // defined(_MJX_CLANG) || defined(_MJX_MSVC)
            }

            static void _Move(wchar_t* const _Dest, const wchar_t* const _Src, const size_t _Count) noexcept {
#ifdef _MJX_CLANG
                __builtin_wmemmove(_Dest, _Src, _Count);
#else // ^^^ Clang ^^^ / vvv GCC or MSVC vvv
                ::wmemmove(_Dest, _Src, _Count);
#endif // _MJX_CLANG
            }

            static void _Copy(wchar_t* const _Dest, const wchar_t* const _Src, const size_t _Count) noexcept {
#ifdef _MJX_CLANG
                __builtin_wmemcpy(_Dest, _Src, _Count);
#else // ^^^ Clang ^^^ / vvv GCC or MSVC vvv
                ::wmemcpy(_Dest, _Src, _Count);
#endif // _MJX_CLANG
            }

            static int _Compare(const wchar_t* const _Left, const wchar_t* const _Right, const size_t _Count) noexcept {
#if defined(_MJX_CLANG) || defined(_MJX_MSVC)
                return __builtin_wmemcmp(_Left, _Right, _Count);
#else // ^^^ Clang or MSVC ^^^ / vvv GCC vvv
                return ::wmemcmp(_Left, _Right, _Count);
#endif // defined(_MJX_CLANG) || defined(_MJX_MSVC)
            }

            static size_t _Length(const wchar_t* const _Str) noexcept {
#if defined(_MJX_CLANG) || defined(_MJX_MSVC)
                return __builtin_wcslen(_Str);
#else // ^^^ Clang or MSVC ^^^ / vvv GCC vvv
                return ::wcslen(_Str);
#endif // defined(_MJX_CLANG) || defined(_MJX_MSVC)
            }

            static size_t _Find(
                const wchar_t* const _Haystack, const size_t _Haystack_size, const wchar_t _Needle) noexcept {
                const wchar_t* const _Match =
#if defined(_MJX_CLANG) || defined(_MJX_MSVC)
                    __builtin_wmemchr(_Haystack, _Needle, _Haystack_size);
#else // ^^^ Clang or MSVC ^^^ / vvv GCC vvv
                    ::wmemchr(_Haystack, _Needle, _Haystack_size);
#endif // defined(_MJX_CLANG) || defined(_MJX_MSVC)
                return _Match ? _Match - _Haystack : static_cast<size_t>(-1);
            }
        };

        template <class _Elem>
        using _Char_traits = ::std::conditional_t<sizeof(_Elem) == 1,
            _Narrow_char_traits<_Elem>, _Wide_char_traits>;
    } // namespace mjstr_impl
} // namespace mjx

#endif // _MJSTR_IMPL_CHAR_TRAITS_HPP_