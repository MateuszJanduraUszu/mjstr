// conversion.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_IMPL_CONVERSION_HPP_
#define _MJSTR_IMPL_CONVERSION_HPP_
#include <cstddef>
#include <mjstr/string.hpp>
#include <mjstr/string_view.hpp>
#include <type_traits>
#ifdef _MJX_WINDOWS
#include <mjstr/impl/tinywin.hpp>
#else // ^^^ _MJX_WINDOWS ^^^ / vvv _MJX_LINUX vvv
#include <cstdlib>
#endif // _MJX_WINDOWS

namespace mjx {
    namespace mjstr_impl {
        template <class _Multibyte>
        struct _Multibyte_to_wide_traits { // traits for conversion from byte/UTF-8 to Unicode
            using _Intern_type = _Multibyte;
            using _Extern_type = wchar_t;

            static size_t _Required_buffer_size(const _Multibyte* const _Str, const size_t _Size) noexcept {
                // calculate the required buffer size for _Str conversion (null-terminator excluded)
                if (_Size == 0) {
                    return 0;
                }

#ifdef _MJX_WINDOWS
                const int _Req_size = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                    reinterpret_cast<const char*>(_Str), static_cast<int>(_Size), nullptr, 0);
                return _Req_size > 0 ? static_cast<size_t>(_Req_size) : static_cast<size_t>(-1);
#else // ^^^ _MJX_WINDOWS ^^^ / vvv _MJX_LINUX vvv
                (void) _Size;
                return ::mbstowcs(nullptr, reinterpret_cast<const char*>(_Str), 0);
#endif // _MJX_WINDOWS
            }

            static bool _Convert(const _Multibyte* const _Str,
                const size_t _Str_size, wchar_t* const _Buf, const size_t _Buf_size) noexcept {
                // convert multibyte character sequence to wide characters
#ifdef _MJX_WINDOWS
                return static_cast<size_t>(::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                    reinterpret_cast<const char*>(_Str), static_cast<int>(_Str_size),
                        _Buf, static_cast<int>(_Buf_size))) == _Buf_size;
#else // ^^^ _MJX_WINDOWS ^^^ / vvv _MJX_LINUX vvv
                (void) _Str_size;
                return ::mbstowcs(_Buf, reinterpret_cast<const char*>(_Str), _Buf_size) == _Buf_size;
#endif // _MJX_WINDOWS
            }
        };

        template <class _Multibyte>
        struct _Wide_to_multibyte_traits { // traits for conversion from Unicode to byte/UTF-8
            using _Intern_type = wchar_t;
            using _Extern_type = _Multibyte;
            
            static size_t _Required_buffer_size(const wchar_t* const _Str, const size_t _Size) noexcept {
                // calculate the required buffer size for _Str conversion (null-terminator excluded)
                if (_Size == 0) {
                    return 0;
                }

#ifdef _MJX_WINDOWS
                const int _Req_size = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
                    _Str, static_cast<int>(_Size), nullptr, 0, nullptr, nullptr);
                return _Req_size > 0 ? static_cast<size_t>(_Req_size) : static_cast<size_t>(-1);
#else // ^^^ _MJX_WINDOWS ^^^ / vvv _MJX_LINUX vvv
                (void) _Size;
                return ::wcstombs(nullptr, _Str, 0);
#endif // _MJX_WINDOWS
            }

            static bool _Convert(const wchar_t* const _Str,
                const size_t _Str_size, _Multibyte* const _Buf, const size_t _Buf_size) noexcept {
                // convert wide character sequence to multibyte
#ifdef _MJX_WINDOWS
                return static_cast<size_t>(::WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS,
                    _Str, static_cast<int>(_Str_size), reinterpret_cast<char*>(_Buf),
                        static_cast<int>(_Buf_size), nullptr, nullptr)) == _Buf_size;
#else // ^^^ _MJX_WINDOWS ^^^ / vvv _MJX_LINUX vvv
                (void) _Str_size;
                return ::wcstombs(reinterpret_cast<char*>(_Buf), _Str, _Buf_size) == _Buf_size;
#endif // _MJX_WINDOWS
            }
        };

        template <class _Extern_char, class _Intern_char>
        using _Choose_cvt_traits = ::std::conditional_t<::std::is_same_v<_Intern_char, wchar_t>,
            _Wide_to_multibyte_traits<_Extern_char>, _Multibyte_to_wide_traits<_Intern_char>>;

        template <class _Extern_char, class _Intern_char>
        inline string<_Extern_char> _Convert_string(const _Intern_char* const _Str, const size_t _Size) {
            using _Traits          = _Choose_cvt_traits<_Extern_char, _Intern_char>;
            using _Str_t           = string<_Extern_char>;
            const size_t _Buf_size = _Traits::_Required_buffer_size(_Str, _Size);
            if (_Buf_size == 0 || _Buf_size == static_cast<size_t>(-1)) {
                // no conversion needed or an error occured, break
                return _Str_t{};
            }

            _Str_t _Buf(_Buf_size, _Extern_char{0});
            return _Traits::_Convert(_Str, _Size, _Buf.data(), _Buf_size) ? ::std::move(_Buf) : _Str_t{};
        }

        template <class _Extern_char, class _Intern_char>
        inline size_t _Convert_string_length(const string_view<_Intern_char> _Str) noexcept {
            using _Traits = _Choose_cvt_traits<_Extern_char, _Intern_char>;
            return _Traits::_Required_buffer_size(_Str.data(), _Str.size());
        }
    } // namespace mjstr_impl
} // namespace mjx

#endif // _MJSTR_IMPL_CONVERSION_HPP_