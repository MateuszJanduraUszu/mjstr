// api.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_API_HPP_
#define _MJSTR_API_HPP_

#ifdef _MJX_WINDOWS
// Windows libraries (DLLs) require an additional attribute
#ifdef _MJSTR_BUILD
#define _MJSTR_API __declspec(dllexport)
#else // ^^^ _MJSTR_BUILD ^^^ / vvv !_MJSTR_BUILD vvv
#define _MJSTR_API __declspec(dllimport)
#endif // _MJSTR_BUILD
#else // ^^^ _MJX_WINDOWS ^^^ / vvv _MJX_LINUX vvv
#define _MJSTR_API
#endif // _MJX_WINDOWS
#endif // _MJSTR_API_HPP_