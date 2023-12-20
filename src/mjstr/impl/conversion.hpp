// conversion.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_IMPL_CONVERSION_HPP_
#define _MJSTR_IMPL_CONVERSION_HPP_
#include <cstddef>
#include <mjstr/impl/tinywin.hpp>
#include <mjstr/string.hpp>
#include <mjstr/string_view.hpp>
#include <type_traits>

namespace mjx {
    namespace mjstr_impl {
        template <class _Multibyte>
        struct _Multibyte_to_wide_traits { // traits for conversion from byte/UTF-8 to Unicode
            using _Intern_type = _Multibyte;
            using _Extern_type = wchar_t;

            static size_t _Required_buffer_size(const _Multibyte* const _Str, const size_t _Size) noexcept {
                if (_Size == 0) {
                    return 0;
                }

                return static_cast<size_t>(::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                    reinterpret_cast<const char*>(_Str), static_cast<int>(_Size), nullptr, 0));
            }

            static bool _Convert(const _Multibyte* const _Str,
                const size_t _Str_size, wchar_t* const _Buf, const size_t _Buf_size) noexcept {
                return static_cast<size_t>(::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                    reinterpret_cast<const char*>(_Str), static_cast<int>(_Str_size),
                        _Buf, static_cast<int>(_Buf_size))) == _Buf_size;
            }
        };

        template <class _Multibyte>
        struct _Wide_to_multibyte_traits { // traits for conversion from Unicode to byte/UTF-8
            using _Intern_type = wchar_t;
            using _Extern_type = _Multibyte;
            
            static size_t _Required_buffer_size(const wchar_t* const _Str, const size_t _Size) noexcept {
                if (_Size == 0) {
                    return 0;
                }

                return static_cast<size_t>(::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
                    _Str, static_cast<int>(_Size), nullptr, 0, nullptr, nullptr));
            }

            static bool _Convert(const wchar_t* const _Str,
                const size_t _Str_size, _Multibyte* const _Buf, const size_t _Buf_size) noexcept {
                return static_cast<size_t>(::WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS,
                    _Str, static_cast<int>(_Str_size), reinterpret_cast<char*>(_Buf),
                        static_cast<int>(_Buf_size), nullptr, nullptr)) == _Buf_size;
            }
        };

        template <class _Traits>
        inline string<typename _Traits::_Extern_type> _Convert_string(
            const typename _Traits::_Intern_type* const _Str, const size_t _Size) {
            using _Extern_char_t   = typename _Traits::_Extern_type;
            using _Str_t           = string<_Extern_char_t>;
            const size_t _Buf_size = _Traits::_Required_buffer_size(_Str, _Size);
            if (_Buf_size == 0) { // no conversion needed or an error occured, break
                return _Str_t{};
            }

            _Str_t _Buf(_Buf_size, _Extern_char_t{0});
            return _Traits::_Convert(_Str, _Size, _Buf.data(), _Buf_size) ? ::std::move(_Buf) : _Str_t{};
        }
    } // namespace mjstr_impl
} // namespace mjx

#endif // _MJSTR_IMPL_CONVERSION_HPP_