// tinywin.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_IMPL_TINYWIN_HPP_
#define _MJSTR_IMPL_TINYWIN_HPP_

#ifdef _MJX_WINDOWS
#define WIN32_LEAN_AND_MEAN
#define NOSERVICE
#include <Windows.h>
#endif // _MJX_WINDOWS
#endif // _MJSTR_IMPL_TINYWIN_HPP_