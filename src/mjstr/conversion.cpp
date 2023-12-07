// conversion.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <mjstr/conversion.hpp>
#include <mjstr/impl/conversion.hpp>
#include <type_traits>

namespace mjx {
    byte_string to_byte_string(const utf8_string_view _Str) noexcept {
        return byte_string{reinterpret_cast<const byte_t*>(_Str.data()), _Str.size()};
    }

    byte_string to_byte_string(const unicode_string_view _Str) noexcept {
        return mjstr_impl::_Convert_string<
            mjstr_impl::_Wide_to_multibyte_traits<byte_t>>(_Str.data(), _Str.size());
    }

    utf8_string to_utf8_string(const byte_string_view _Str) noexcept {
        return utf8_string{reinterpret_cast<const char*>(_Str.data()), _Str.size()};
    }

    utf8_string to_utf8_string(const unicode_string_view _Str) noexcept {
        return mjstr_impl::_Convert_string<
            mjstr_impl::_Wide_to_multibyte_traits<char>>(_Str.data(), _Str.size());
    }

    unicode_string to_unicode_string(const byte_string_view _Str) noexcept {
        return mjstr_impl::_Convert_string<
            mjstr_impl::_Multibyte_to_wide_traits<byte_t>>(_Str.data(), _Str.size());
    }

    unicode_string to_unicode_string(const utf8_string_view _Str) noexcept {
        return mjstr_impl::_Convert_string<
            mjstr_impl::_Multibyte_to_wide_traits<char>>(_Str.data(), _Str.size());
    }
} // namespace mjx