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
#include <cstdint>
#endif // _MJX_WINDOWS

namespace mjx {
    namespace mjstr_impl {
#ifdef _MJX_LINUX
        // Note: On Linux, Unicode characters are typically stored in UTF-32 encoding,
        //       which means wchar_t should be 4 bytes long. This implementation handles
        //       UCS-4 code points, which also rely on a 4-byte wchar_t type.
        static_assert(sizeof(wchar_t) == 4, "unexpected wchar_t size");

        constexpr size_t _Calculate_utf8_encoded_length(const wchar_t* _Chars, const size_t _Size) noexcept {
            // calculate the length of the UTF-8 encoded string from _Chars
            const wchar_t* const _Last = _Chars + _Size;
            size_t _Count              = 0;
            uint32_t _Code_point;
            for (; _Chars != _Last; ++_Chars) {
                _Code_point = static_cast<uint32_t>(*_Chars);
                if (_Code_point <= 0x7F) { // U+0000...U+007F, single byte
                    ++_Count;
                } else if (_Code_point <= 0x07FF) { // U+0080...U+07FF, two bytes
                    _Count += 2;
                } else if (_Code_point <= 0xFFFF) { // U+0800...U+FFFF, three bytes
                    _Count += 3;
                } else if (_Code_point <= 0x0010'FFFF) { // U+010000...U+01FFFF, four bytes
                    _Count += 4;
                } else { // invalid code point (too big), break
                    return static_cast<size_t>(-1);
                }
            }

            return _Count;
        }

        template <class _Elem>
        constexpr bool _Encode_utf8(const wchar_t* _Chars, const size_t _Size, _Elem* _Buf) noexcept {
            // encode _Chars to UTF-8 and write to _Buf, assumes _Buf can fit all bytes
            const wchar_t* const _Last = _Chars + _Size;
            size_t _Trailing;
            uint32_t _Code_point;
            for (; _Chars != _Last; ++_Chars) {
                _Code_point = static_cast<uint32_t>(*_Chars);
                if (_Code_point <= 0x7F) { // U+0000...U+007F, single byte
                    *_Buf++   = static_cast<_Elem>(_Code_point);
                    _Trailing = 0;
                } else if (_Code_point <= 0x07FF) { // U+0080...U+07FF, two bytes
                    *_Buf++   = static_cast<_Elem>(0xC0 | (_Code_point >> 6));
                    _Trailing = 1;
                } else if (_Code_point <= 0xFFFF) { // U+0800...U+FFFF, three bytes
                    *_Buf++   = static_cast<_Elem>(0xE0 | (_Code_point >> 12));
                    _Trailing = 2;
                } else if (_Code_point <= 0x0010'FFFF) { // U+010000...U+10FFFF, four bytes
                    *_Buf++   = static_cast<_Elem>(0xF0 | (_Code_point >> 18));
                    _Trailing = 3;
                } else { // invalid code point (too big), break
                    return false;
                }

                for (; _Trailing > 0; ++_Buf) { // append trailing bytes, if any
                    *_Buf = static_cast<_Elem>(0x80 | ((_Code_point >> (6 * --_Trailing)) & 0x3F));
                }
            }

            return true;
        }

        template <class _Elem>
        constexpr size_t _Calculate_utf8_decoded_length(const _Elem* _Bytes, const size_t _Size) noexcept {
            // calculate the length of the UTF-8 decoded string from _Bytes
            const _Elem* const _Last = _Bytes + _Size;
            size_t _Count            = 0;
            size_t _Trailing;
            uint8_t _Byte;
            for (; _Bytes != _Last; ++_Count) {
                _Byte = static_cast<uint8_t>(*_Bytes++);
                if (_Byte <= 0x7F) { // 0XXXXXXX pattern, single byte
                    _Trailing = 0;
                } else if (_Byte >= 0xC0 && _Byte <= 0xDF) { // 110XXXXX pattern, two bytes
                    _Trailing = 1;
                } else if (_Byte <= 0xEF) { // 1110XXXX pattern, three bytes
                    _Trailing = 2;
                } else if (_Byte <= 0xF7) { // 11110XXX pattern, four bytes
                    _Trailing = 3;
                } else { // invalid leading byte, break
                    return static_cast<size_t>(-1);
                }

                if (_Last - _Bytes < _Trailing) { // incomplete sequence, break
                    return static_cast<size_t>(-1);
                }

                _Bytes += _Trailing;
            }

            return _Count;
        }

        template <class _Elem>
        constexpr bool _Decode_utf8(const _Elem* _Bytes, const size_t _Size, wchar_t* _Buf) noexcept {
            // decode _Bytes to Unicode and write to _Buf, assumes _Buf can fit all characters
            const _Elem* const _Last = _Bytes + _Size;
            size_t _Trailing;
            uint8_t _Byte;
            uint32_t _Least_code_point;
            uint32_t _Code_point;
            while (_Bytes != _Last) {
                _Byte = static_cast<uint8_t>(*_Bytes++);
                if (_Byte <= 0x7F) { // 0XXXXXXX pattern, single byte
                    _Least_code_point = 0x00;
                    _Code_point       = static_cast<uint32_t>(_Byte);
                    _Trailing         = 0;
                } else if (_Byte >= 0xC0 && _Byte <= 0xDF) { // 110XXXXX pattern, two bytes
                    _Least_code_point = 0x80;
                    _Code_point       = static_cast<uint32_t>(_Byte & 0x1F);
                    _Trailing         = 1;
                } else if (_Byte <= 0xEF) { // 1110XXXX pattern, three bytes
                    _Least_code_point = 0x0800;
                    _Code_point       = static_cast<uint32_t>(_Byte & 0x0F);
                    _Trailing         = 2;
                } else if (_Byte <= 0xF7) { // 11110XXX pattern, four bytes
                    _Least_code_point = 0x0001'0000;
                    _Code_point       = static_cast<uint32_t>(_Byte & 0x07);
                    _Trailing         = 3;
                } else { // invalid leading byte, break
                    return false;
                }

                if (_Last - _Bytes < _Trailing) { // incomplete sequence, break
                    return false;
                }

                while (_Trailing-- > 0) {
                    _Byte = static_cast<uint8_t>(*_Bytes++);
                    if (_Byte < 0x80 || _Byte > 0xBF) { // invalid trailing byte, break
                        return false;
                    }

                    _Code_point = (_Code_point << 6) | (_Byte & 0x3F);
                }

                if (_Code_point < _Least_code_point) { // overlong encoding detected, break
                    return false;
                }

                *_Buf++ = static_cast<wchar_t>(_Code_point);
            }

            return true;
        }
#endif // _MJX_LINUX

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
                return _Calculate_utf8_decoded_length(_Str, _Size);
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
                (void) _Buf_size;
                return _Decode_utf8(_Str, _Str_size, _Buf);
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
                return _Calculate_utf8_encoded_length(_Str, _Size);
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
                (void) _Buf_size;
                return _Encode_utf8(_Str, _Str_size, _Buf);
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