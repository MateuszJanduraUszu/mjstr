// conversion.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <mjstr/conversion.hpp>
#include <mjstr/impl/conversion.hpp>

namespace mjx {
    size_t to_byte_string_length(const utf8_string_view _Str) noexcept {
        return _Str.size();
    }

    size_t to_byte_string_length(const unicode_string_view _Str) noexcept {
        return mjstr_impl::_Convert_string_length<byte_t>(_Str);
    }

    size_t to_utf8_string_length(const byte_string_view _Str) noexcept {
        return _Str.size();
    }

    size_t to_utf8_string_length(const unicode_string_view _Str) noexcept {
        return mjstr_impl::_Convert_string_length<char>(_Str);
    }

    size_t to_unicode_string_length(const byte_string_view _Str) noexcept {
        return mjstr_impl::_Convert_string_length<wchar_t>(_Str);
    }

    size_t to_unicode_string_length(const utf8_string_view _Str) noexcept {
        return mjstr_impl::_Convert_string_length<wchar_t>(_Str);
    }

    byte_string to_byte_string(const utf8_string_view _Str) {
        return byte_string{reinterpret_cast<const byte_t*>(_Str.data()), _Str.size()};
    }

    byte_string to_byte_string(const unicode_string_view _Str) {
        return mjstr_impl::_Convert_string<byte_t>(_Str.data(), _Str.size());
    }

    utf8_string to_utf8_string(const byte_string_view _Str) {
        return utf8_string{reinterpret_cast<const char*>(_Str.data()), _Str.size()};
    }

    utf8_string to_utf8_string(const unicode_string_view _Str) {
        return mjstr_impl::_Convert_string<char>(_Str.data(), _Str.size());
    }

    unicode_string to_unicode_string(const byte_string_view _Str) {
        return mjstr_impl::_Convert_string<wchar_t>(_Str.data(), _Str.size());
    }

    unicode_string to_unicode_string(const utf8_string_view _Str) {
        return mjstr_impl::_Convert_string<wchar_t>(_Str.data(), _Str.size());
    }
} // namespace mjx