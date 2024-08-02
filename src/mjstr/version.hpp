// version.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_VERSION_HPP_
#define _MJSTR_VERSION_HPP_
#include <mjmem/version.hpp>

// Note: The core versioning macros are defined in <mjmem/version.hpp>. These macros serve as the basis
//       for versioning in all modules. If a core library that forms the foundation for MJMEM is ever
//       introduced, this inclusion should be updated accordingly.

// defines the latest MJSTR library version, synchronized with the version specified in 'res/mjstr.rc'
#define _MJSTR_VERSION_MAJOR 2ULL
#define _MJSTR_VERSION_MINOR 0ULL
#define _MJSTR_VERSION_PATCH 0ULL
#define _MJSTR_VERSION       _MJX_ENCODE_VERSION(_MJSTR_VERSION_MAJOR, _MJSTR_VERSION_MINOR, _MJSTR_VERSION_PATCH)

// checks whether the current version is greater than or equal to the specified version
#define _MJSTR_VERSION_SUPPORTED(_Major, _Minor, _Patch) \
    (_MJSTR_VERSION >= _MJX_ENCODE_VERSION(_Major, _Minor, _Patch))
#endif // _MJSTR_VERSION_HPP_