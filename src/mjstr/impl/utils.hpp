// utils.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_IMPL_UTILS_HPP_
#define _MJSTR_IMPL_UTILS_HPP_
#include <crtdbg.h>

#ifdef _DEBUG
#define _INTERNAL_ASSERT(_Cond, _Msg)                                        \
    if (!(_Cond)) {                                                          \
        ::_CrtDbgReport(_CRT_ERROR, __FILE__, __LINE__, __FUNCTION__, _Msg); \
    }
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
#define _INTERNAL_ASSERT(_Cond, _Msg)
#endif // _DEBUG

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
    } // namespace mjstr_impl
} // namespace mjx

#endif // _MJSTR_IMPL_UTILS_HPP_