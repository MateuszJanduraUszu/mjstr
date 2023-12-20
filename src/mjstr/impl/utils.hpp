// utils.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_IMPL_UTILS_HPP_
#define _MJSTR_IMPL_UTILS_HPP_
#include <crtdbg.h>

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

#endif // _MJSTR_IMPL_UTILS_HPP_