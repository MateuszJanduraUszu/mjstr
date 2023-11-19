// byte_string.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <mjstr/impl/string.hpp>
#include <mjstr/impl/utils.hpp>
#include <mjstr/string.hpp>

namespace mjx {
    template <class _Elem>
    basic_string_view<_Elem>::basic_string_view() noexcept : _Mydata(nullptr), _Mysize(0) {}

    template <class _Elem>
    basic_string_view<_Elem>::basic_string_view(const_pointer _Ptr) noexcept
        : _Mydata(_Ptr), _Mysize(mjstr_impl::_String_traits<_Elem>::_Length(_Ptr)) {}

    template <class _Elem>
    basic_string_view<_Elem>::basic_string_view(const_pointer _Ptr, const size_type _Count) noexcept
        : _Mydata(_Ptr), _Mysize(_Count) {}

    template <class _Elem>
    typename basic_string_view<_Elem>::const_reference
        basic_string_view<_Elem>::operator[](const size_type _Idx) const noexcept {
        // no bounds checking is performed, the behavior is undefined if _Idx >= size()
        return _Mydata[_Idx];
    }

    template <class _Elem>
    typename basic_string_view<_Elem>::const_iterator basic_string_view<_Elem>::begin() const noexcept {
#ifdef _DEBUG
        return const_iterator{_Mydata, _Mydata + _Mysize};
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        return const_iterator{_Mydata};
#endif // _DEBUG
    }

    template <class _Elem>
    typename basic_string_view<_Elem>::const_iterator basic_string_view<_Elem>::end() const noexcept {
#ifdef _DEBUG
        return const_iterator{_Mydata + _Mysize, _Mydata + _Mysize};
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        return const_iterator{_Mydata + _Mysize};
#endif // _DEBUG
    }

    template <class _Elem>
    typename basic_string_view<_Elem>::const_reference
        basic_string_view<_Elem>::at(const size_type _Idx) const noexcept {
        _INTERNAL_ASSERT(_Idx < _Mysize, "attempt to access non-existent element");
        return _Mydata[_Idx];
    }

    template <class _Elem>
    typename basic_string_view<_Elem>::const_reference basic_string_view<_Elem>::front() const noexcept {
        _INTERNAL_ASSERT(_Mysize > 0, "attempt to access non-existent element");
        return _Mydata[0];
    }

    template <class _Elem>
    typename basic_string_view<_Elem>::const_reference basic_string_view<_Elem>::back() const noexcept {
        _INTERNAL_ASSERT(_Mysize > 0, "attempt to access non-existent element");
        return _Mydata[_Mysize - 1];;
    }

    template <class _Elem>
    typename basic_string_view<_Elem>::const_pointer basic_string_view<_Elem>::data() const noexcept {
        return _Mydata;
    }

    template <class _Elem>
    typename basic_string_view<_Elem>::size_type basic_string_view<_Elem>::size() const noexcept {
        return _Mysize;
    }

    template <class _Elem>
    bool basic_string_view<_Elem>::empty() const noexcept {
        return _Mysize == 0;
    }

    template <class _Elem>
    bool basic_string_view<_Elem>::remove_prefix(const size_type _Count) noexcept {
        if (_Count > _Mysize) {
            return false;
        }

        _Mydata += _Count;
        _Mysize -= _Count;
        return true;
    }

    template <class _Elem>
    bool basic_string_view<_Elem>::remove_suffix(const size_type _Count) noexcept {
        if (_Count > _Mysize) {
            return false;
        }

        _Mysize -= _Count;
        return true;
    }

    template <class _Elem>
    void basic_string_view<_Elem>::swap(basic_string_view& _Other) noexcept {
        const basic_string_view _Temp = *this;
        *this                         = _Other;
        _Other                        = _Temp;
    }

    template <class _Elem>
    typename basic_string_view<_Elem>::size_type basic_string_view<_Elem>::copy(
        pointer _Dest, size_type _Count, const size_type _Off) const noexcept {
        if (_Off >= _Mysize) {
            return 0;
        }

        _Count = mjstr_impl::_Min(_Count, _Mysize - _Off);
        mjstr_impl::_String_traits<_Elem>::_Copy(_Dest, _Mydata + _Off, _Count);
        return _Count;
    }

    template <class _Elem>
    basic_string_view<_Elem> basic_string_view<_Elem>::substr(
        const size_type _Off, size_type _Count) const noexcept {
        if (_Off >= _Mysize) {
            return basic_string_view{};
        }

        _Count = mjstr_impl::_Min(_Count, _Mysize - _Off); // trim number of characters
        return basic_string_view{_Mydata + _Off, _Count};
    }

    template <class _Elem>
    int basic_string_view<_Elem>::compare(const basic_string_view _Str) const noexcept {
        return mjstr_impl::_String_traits<_Elem>::_Compare(_Mydata, _Mysize, _Str._Mydata, _Str._Mysize);
    }

    template <class _Elem>
    int basic_string_view<_Elem>::compare(
        const size_type _Off, const size_type _Count, const basic_string_view _Str) const noexcept {
        return substr(_Off, _Count).compare(_Str);
    }

    template <class _Elem>
    int basic_string_view<_Elem>::compare(const size_type _Off, const size_type _Count,
        const basic_string_view _Str, const size_type _Str_off, const size_type _Str_count) const noexcept {
        return substr(_Off, _Count).compare(_Str.substr(_Str_off, _Str_count));
    }

    template <class _Elem>
    int basic_string_view<_Elem>::compare(const_pointer _Ptr) const noexcept {
        return compare(basic_string_view{_Ptr});
    }

    template <class _Elem>
    int basic_string_view<_Elem>::compare(
        const size_type _Off, const size_type _Count, const_pointer _Ptr) const noexcept {
        return substr(_Off, _Count).compare(basic_string_view{_Ptr});
    }

    template <class _Elem>
    int basic_string_view<_Elem>::compare(const size_type _Off, const size_type _Count,
        const_pointer _Ptr, const size_type _Ptr_count) const noexcept {
        return substr(_Off, _Count).compare(basic_string_view{_Ptr, _Ptr_count});
    }

    template <class _Elem>
    bool basic_string_view<_Elem>::starts_with(const basic_string_view _Str) const noexcept {
        if (_Str._Mysize > _Mysize) {
            return false;
        }

        return mjstr_impl::_String_traits<_Elem>::_Equal(_Mydata, _Str._Mydata, _Str._Mysize);
    }

    template <class _Elem>
    bool basic_string_view<_Elem>::starts_with(const value_type _Ch) const noexcept {
        return _Mysize > 0 ? _Mydata[0] == _Ch : false;
    }

    template <class _Elem>
    bool basic_string_view<_Elem>::starts_with(const_pointer _Ptr) const noexcept {
        return starts_with(basic_string_view{_Ptr});
    }

    template <class _Elem>
    bool basic_string_view<_Elem>::ends_with(const basic_string_view _Str) const noexcept {
        if (_Str._Mysize > _Mysize) {
            return false;
        }

        return mjstr_impl::_String_traits<_Elem>::_Equal(
            _Mydata + (_Mysize - _Str._Mysize), _Str._Mydata, _Str._Mysize);
    }

    template <class _Elem>
    bool basic_string_view<_Elem>::ends_with(const value_type _Ch) const noexcept {
        return _Mysize > 0 ? _Mydata[_Mysize - 1] == _Ch : false;
    }

    template <class _Elem>
    bool basic_string_view<_Elem>::ends_with(const_pointer _Ptr) const noexcept {
        return ends_with(basic_string_view{_Ptr});
    }

    template <class _Elem>
    bool basic_string_view<_Elem>::contains(const basic_string_view _Str) const noexcept {
        return find(_Str) != npos;
    }

    template <class _Elem>
    bool basic_string_view<_Elem>::contains(const value_type _Ch) const noexcept {
        return find(_Ch) != npos;
    }

    template <class _Elem>
    bool basic_string_view<_Elem>::contains(const_pointer _Ptr) const noexcept {
        return find(_Ptr) != npos;
    }

    template <class _Elem>
    typename basic_string_view<_Elem>::size_type basic_string_view<_Elem>::find(
        const basic_string_view _Str, const size_type _Off) const noexcept {
        if (_Off >= _Mysize || _Mysize - _Off < _Str._Mysize) {
            return npos;
        }

        if (_Str._Mysize == 0) { // empty string is always considered as found
            return _Off;
        }
        
        if (_Off == 0) { // offset has no meaning
            return mjstr_impl::_String_traits<_Elem>::_Find(_Mydata, _Str._Mydata);
        } else {
            const size_type _Idx = mjstr_impl::_String_traits<_Elem>::_Find(_Mydata + _Off, _Str._Mydata);
            return _Idx != npos ? _Idx + _Off : npos;
        }
    }

    template <class _Elem>
    typename basic_string_view<_Elem>::size_type basic_string_view<_Elem>::find(
        const value_type _Ch, const size_type _Off) const noexcept {
        if (_Off >= _Mysize) {
            return npos;
        }

        if (_Off == 0) { // offset has no meaning
            return mjstr_impl::_String_traits<_Elem>::_Find(_Mydata, _Ch);
        } else {
            const size_type _Idx = mjstr_impl::_String_traits<_Elem>::_Find(_Mydata + _Off, _Ch);
            return _Idx != npos ? _Idx + _Off : npos;
        }
    }

    template <class _Elem>
    typename basic_string_view<_Elem>::size_type basic_string_view<_Elem>::find(
        const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept {
        if (_Off >= _Mysize) {
            return npos;
        }

        if (_Count == 0) { // empty string is always considered as found
            return _Off;
        }

        if (_Off == 0) { // offset has no meaning
            return mjstr_impl::_String_traits<_Elem>::_Find_n(_Mydata, _Mysize, _Ptr, _Count);
        } else {
            const size_type _Idx = mjstr_impl::_String_traits<_Elem>::_Find_n(
                _Mydata + _Off, _Mysize - _Off, _Ptr, _Count);
            return _Idx != npos ? _Idx + _Off : npos;
        }
    }

    template <class _Elem>
    typename basic_string_view<_Elem>::size_type basic_string_view<_Elem>::find(
        const_pointer _Ptr, const size_type _Off) const noexcept {
        return find(basic_string_view{_Ptr}, _Off);
    }

    template <class _Elem>
    typename basic_string_view<_Elem>::size_type basic_string_view<_Elem>::rfind(
        const basic_string_view _Str, const size_type _Off) const noexcept {
        if (_Str._Mysize > _Mysize) {
            return npos;
        }

        if (_Str._Mysize == 0) { // empty string is always considered as found
            return mjstr_impl::_Min(_Off, _Mysize - 1);
        }

        return mjstr_impl::_String_traits<_Elem>::_Rfind(
            _Mydata, mjstr_impl::_Min(_Off, _Mysize - _Str.size()), _Str._Mydata, _Str._Mysize);
    }

    template <class _Elem>
    typename basic_string_view<_Elem>::size_type basic_string_view<_Elem>::rfind(
        const value_type _Ch, const size_type _Off) const noexcept {
        if (_Mysize == 0) {
            return npos;
        }

        return mjstr_impl::_String_traits<_Elem>::_Rfind(_Mydata, mjstr_impl::_Min(_Off, _Mysize - 1), _Ch);
    }

    template <class _Elem>
    typename basic_string_view<_Elem>::size_type basic_string_view<_Elem>::rfind(
        const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept {
        return rfind(basic_string_view{_Ptr, _Count}, _Off);
    }

    template <class _Elem>
    typename basic_string_view<_Elem>::size_type basic_string_view<_Elem>::rfind(
        const_pointer _Ptr, const size_type _Off) const noexcept {
        return rfind(basic_string_view{_Ptr}, _Off);
    }

    template _MJSTR_API class basic_string_view<byte_t>;
    template _MJSTR_API class basic_string_view<char>;
    template _MJSTR_API class basic_string_view<wchar_t>;

    template <class _Elem>
    bool operator==(const basic_string_view<_Elem> _Left, const basic_string_view<_Elem> _Right) noexcept {
        return _Left.compare(_Right) == 0;
    }

    template _MJSTR_API bool operator==(const byte_string_view, const byte_string_view) noexcept;
    template _MJSTR_API bool operator==(const utf8_string_view, const utf8_string_view) noexcept;
    template _MJSTR_API bool operator==(const unicode_string_view, const unicode_string_view) noexcept;

    template <class _Elem>
    bool operator==(const basic_string_view<_Elem> _Left, const _Elem* const _Right) noexcept {
        return _Left.compare(_Right) == 0;
    }

    template _MJSTR_API bool operator==(const byte_string_view, const byte_t* const) noexcept;
    template _MJSTR_API bool operator==(const utf8_string_view, const char* const) noexcept;
    template _MJSTR_API bool operator==(const unicode_string_view, const wchar_t* const) noexcept;

    template <class _Elem>
    bool operator==(const _Elem* const _Left, const basic_string_view<_Elem> _Right) noexcept {
        return _Right.compare(_Left) == 0;
    }
    
    template _MJSTR_API bool operator==(const byte_t* const, const byte_string_view) noexcept;
    template _MJSTR_API bool operator==(const char* const, const utf8_string_view) noexcept;
    template _MJSTR_API bool operator==(const wchar_t* const, const unicode_string_view) noexcept;

    template <class _Elem>
    bool operator!=(const basic_string_view<_Elem> _Left, const basic_string_view<_Elem> _Right) noexcept {
        return _Left.compare(_Right) != 0;
    }

    template _MJSTR_API bool operator!=(const byte_string_view, const byte_string_view) noexcept;
    template _MJSTR_API bool operator!=(const utf8_string_view, const utf8_string_view) noexcept;
    template _MJSTR_API bool operator!=(const unicode_string_view, const unicode_string_view) noexcept;

    template <class _Elem>
    bool operator!=(const basic_string_view<_Elem> _Left, const _Elem* const _Right) noexcept {
        return _Left.compare(_Right) != 0;
    }

    template _MJSTR_API bool operator!=(const byte_string_view, const byte_t* const) noexcept;
    template _MJSTR_API bool operator!=(const utf8_string_view, const char* const) noexcept;
    template _MJSTR_API bool operator!=(const unicode_string_view, const wchar_t* const) noexcept;

    template <class _Elem>
    bool operator!=(const _Elem* const _Left, const basic_string_view<_Elem> _Right) noexcept {
        return _Right.compare(_Left) != 0;
    }

    template _MJSTR_API bool operator!=(const byte_t* const, const byte_string_view) noexcept;
    template _MJSTR_API bool operator!=(const char* const, const utf8_string_view) noexcept;
    template _MJSTR_API bool operator!=(const wchar_t* const, const unicode_string_view) noexcept;
} // namespace mjx