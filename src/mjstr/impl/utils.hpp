// utils.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_IMPL_UTILS_HPP_
#define _MJSTR_IMPL_UTILS_HPP_
#include <crtdbg.h>
#include <stdexcept>
#include <type_traits>

// generic assert macro, useful in debug mode
#define _INTERNAL_ASSERT(_Cond, _Msg)                                        \
    if (!(_Cond)) {                                                          \
        ::_CrtDbgReport(_CRT_ERROR, __FILE__, __LINE__, __FUNCTION__, _Msg); \
    }

#ifdef __has_builtin
#define _HAS_BUILTIN(_Builtin) __has_builtin(_Builtin)
#else // ^^^ __has_builtin ^^^ / vvv !__has_builtin vvv
#define _HAS_BUILTIN(_Builtin) 0
#endif // __has_builtin

namespace mjx {
    namespace mjstr_impl {
        template <class _Ty>
        constexpr const _Ty& _Min(const _Ty& _Left, const _Ty& _Right) noexcept {
            return _Left <= _Right ? _Left : _Right;
        }

        template <class _Ty>
        constexpr void _Swap(_Ty& _Left, _Ty& _Right) noexcept {
            _Ty _Temp = ::std::move(_Left);
            _Left     = ::std::move(_Right);
            _Right    = ::std::move(_Temp);
            
        }

        template <class _Ty>
        inline _Ty* _Allocate(const size_t _Count) {
            return static_cast<_Ty*>(::operator new(_Count * sizeof(_Ty))); // may throw
        }

        template <class _Ty>
        inline void _Deallocate(_Ty* const _Ptr, const size_t _Count) noexcept {
            ::operator delete(_Ptr, _Count * sizeof(_Ty));
        }

        __declspec(noreturn) inline void _Throw_length_error(const char* const _Msg) {
            throw ::std::length_error(_Msg);
        }

        __declspec(noreturn) inline void _Throw_out_of_range(const char* const _Msg) {
            throw ::std::out_of_range(_Msg);
        }
    } // namespace mjstr_impl
} // namespace mjx

#endif // _MJSTR_IMPL_UTILS_HPP_