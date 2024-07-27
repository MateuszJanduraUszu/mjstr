// api.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_API_HPP_
#define _MJSTR_API_HPP_

#ifdef _MJSTR_BUILD
#define _MJSTR_API __declspec(dllexport)
#else // ^^^ _MJSTR_BUILD ^^^ / vvv !_MJSTR_BUILD vvv
#define _MJSTR_API __declspec(dllimport)
#endif // _MJSTR_BUILD
#endif // _MJSTR_API_HPP_