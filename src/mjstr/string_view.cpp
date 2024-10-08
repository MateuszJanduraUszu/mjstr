// string_view.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <algorithm>
#include <mjmem/exception.hpp>
#include <mjstr/impl/utils.hpp>
#include <mjstr/string_view.hpp>

namespace mjx {
    template <class _Elem>
    string_view_iterator<_Elem>::string_view_iterator() noexcept
#ifdef _DEBUG
        : _Myptr(nullptr), _Mybegin(nullptr), _Myend(nullptr) {}
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        : _Myptr(nullptr) {}
#endif // _DEBUG

    template <class _Elem>
    string_view_iterator<_Elem>::string_view_iterator(
#ifdef _DEBUG
        pointer _First, pointer _Last) noexcept : _Myptr(_First), _Mybegin(_First), _Myend(_Last) {
        _INTERNAL_ASSERT(_First <= _Last, "invalid iterator bounds");
    }
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        pointer _Ptr) noexcept : _Myptr(_Ptr) {}
#endif // _DEBUG

    template <class _Elem>
    typename string_view_iterator<_Elem>::reference string_view_iterator<_Elem>::operator*() const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr && _Myptr != _Myend, "attempt to dereference invalid iterator");
#endif // _DEBUG
        return *_Myptr;
    }

    template <class _Elem>
    typename string_view_iterator<_Elem>::pointer string_view_iterator<_Elem>::operator->() const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr && _Myptr != _Myend, "attempt to dereference invalid iterator");
#endif // _DEBUG
        return _Myptr;
    }

    template <class _Elem>
    typename string_view_iterator<_Elem>::reference
        string_view_iterator<_Elem>::operator[](const difference_type _Off) const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myend - _Myptr >= _Off, "attempt to access non-existent element");
#endif // _DEBUG
        return _Myptr[_Off];
    }

    template <class _Elem>
    string_view_iterator<_Elem>& string_view_iterator<_Elem>::operator++() noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myend - _Myptr > 0, "attempt to advance iterator that points to the end");
#endif // _DEBUG
        ++_Myptr;
        return *this;
    }

    template <class _Elem>
    string_view_iterator<_Elem> string_view_iterator<_Elem>::operator++(int) noexcept {
        const string_view_iterator _Temp = *this;
        ++*this;
        return _Temp;
    }

    template <class _Elem>
    string_view_iterator<_Elem>& string_view_iterator<_Elem>::operator--() noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myptr - _Mybegin > 0, "attempt to retreat iterator that points to the beginning");
#endif // _DEBUG
        --_Myptr;
        return *this;
    }

    template <class _Elem>
    string_view_iterator<_Elem> string_view_iterator<_Elem>::operator--(int) noexcept {
        const string_view_iterator _Temp = *this;
        --*this;
        return _Temp;
    }

    template <class _Elem>
    string_view_iterator<_Elem>& string_view_iterator<_Elem>::operator+=(const difference_type _Off) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myend - _Myptr >= _Off, "attempt to advance iterator beyond the end");
#endif // _DEBUG
        _Myptr += _Off;
        return *this;
    }

    template <class _Elem>
    string_view_iterator<_Elem>& string_view_iterator<_Elem>::operator-=(const difference_type _Off) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myptr - _Mybegin >= _Off, "attempt to retreat iterator beyond the beginning");
#endif // _DEBUG
        _Myptr -= _Off;
        return *this;
    }

    template <class _Elem>
    string_view_iterator<_Elem> string_view_iterator<_Elem>::operator+(const difference_type _Off) const noexcept {
        string_view_iterator _Temp = *this;
        _Temp                     += _Off;
        return _Temp;
    }

    template <class _Elem>
    string_view_iterator<_Elem> string_view_iterator<_Elem>::operator-(const difference_type _Off) const noexcept {
        string_view_iterator _Temp = *this;
        _Temp                     -= _Off;
        return _Temp;
    }

    template <class _Elem>
    bool string_view_iterator<_Elem>::operator==(const string_view_iterator& _Other) const noexcept {
        return _Myptr == _Other._Myptr;
    }

    template <class _Elem>
    ::std::strong_ordering
        string_view_iterator<_Elem>::operator<=>(const string_view_iterator& _Other) const noexcept {
        return _Myptr <=> _Other._Myptr;
    }

    template class _MJSTR_API string_view_iterator<byte_t>;
    template class _MJSTR_API string_view_iterator<char>;
    template class _MJSTR_API string_view_iterator<wchar_t>;

    template <class _Elem>
    string_view<_Elem>::string_view() noexcept : _Mydata(nullptr), _Mysize(0) {}

    template <class _Elem>
    string_view<_Elem>::string_view(const_pointer _Ptr, const size_type _Count) noexcept
        : _Mydata(_Ptr), _Mysize(_Count) {}

    template <class _Elem>
    string_view<_Elem>::string_view(const_pointer _Ptr) noexcept
        : _Mydata(_Ptr), _Mysize(traits_type::length(_Ptr)) {}

    template <class _Elem>
    typename string_view<_Elem>::const_reference
        string_view<_Elem>::operator[](const size_type _Idx) const noexcept {
        // no bounds checking is performed, the behavior is undefined if _Idx >= size()
        return _Mydata[_Idx];
    }

    template <class _Elem>
    void string_view<_Elem>::_Check_offset(const size_type _Off) const {
        if (_Off >= _Mysize) {
            resource_overrun::raise();
        }
    }

    template <class _Elem>
    typename string_view<_Elem>::const_iterator string_view<_Elem>::begin() const noexcept {
#ifdef _DEBUG
        return const_iterator{_Mydata, _Mydata + _Mysize};
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        return const_iterator{_Mydata};
#endif // _DEBUG
    }

    template <class _Elem>
    typename string_view<_Elem>::const_iterator string_view<_Elem>::end() const noexcept {
#ifdef _DEBUG
        return const_iterator{_Mydata + _Mysize, _Mydata + _Mysize};
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        return const_iterator{_Mydata + _Mysize};
#endif // _DEBUG
    }

    template <class _Elem>
    typename string_view<_Elem>::const_reference string_view<_Elem>::at(const size_type _Idx) const {
        _Check_offset(_Idx);
        return _Mydata[_Idx];
    }

    template <class _Elem>
    typename string_view<_Elem>::const_reference string_view<_Elem>::front() const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Mysize > 0, "attempt to access non-existent element");
#endif // _DEBUG
        return _Mydata[0];
    }

    template <class _Elem>
    typename string_view<_Elem>::const_reference string_view<_Elem>::back() const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Mysize > 0, "attempt to access non-existent element");
#endif // _DEBUG
        return _Mydata[_Mysize - 1];
    }

    template <class _Elem>
    typename string_view<_Elem>::const_pointer string_view<_Elem>::data() const noexcept {
        return _Mydata;
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type string_view<_Elem>::size() const noexcept {
        return _Mysize;
    }

    template <class _Elem>
    bool string_view<_Elem>::empty() const noexcept {
        return _Mysize == 0;
    }

    template <class _Elem>
    void string_view<_Elem>::remove_prefix(const size_type _Count) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Count <= _Mysize, "attempt to remove prefix longer than total string size");
#endif // _DEBUG
        _Mydata += _Count;
        _Mysize -= _Count;
    }

    template <class _Elem>
    void string_view<_Elem>::remove_suffix(const size_type _Count) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Count <= _Mysize, "attempt to remove suffix longer than total string size");
#endif // _DEBUG
        _Mysize -= _Count;
    }

    template <class _Elem>
    void string_view<_Elem>::swap(string_view& _Other) noexcept {
        const string_view _Temp = *this;
        *this                   = _Other;
        _Other                  = _Temp;
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type
        string_view<_Elem>::copy(pointer _Dest, size_type _Count, const size_type _Off) const {
        _Check_offset(_Off);
        _Count = (::std::min)(_Count, _Mysize - _Off);
        traits_type::copy(_Dest, _Mydata + _Off, _Count);
        return _Count;
    }

    template <class _Elem>
    string_view<_Elem> string_view<_Elem>::substr(const size_type _Off, size_type _Count) const {
        _Check_offset(_Off);
        _Count = (::std::min)(_Count, _Mysize - _Off); // adjust the number of returned characters
        return string_view{_Mydata + _Off, _Count};
    }

    template <class _Elem>
    int string_view<_Elem>::compare(const string_view _Str) const noexcept {
        const int _Result = traits_type::compare(_Mydata, _Str._Mydata, (::std::min)(_Mysize, _Str._Mysize));
        if (_Result != 0) { // strings are not equal, no further checks required
            return _Result;
        }

        if (_Mysize == _Str._Mysize) { // strings are equal, so are the sizes
            return 0;
        }

        return _Mysize < _Str._Mysize ? -1 : 1;
    }

    template <class _Elem>
    int string_view<_Elem>::compare(const_pointer _Ptr, const size_type _Count) const noexcept {
        return compare(string_view{_Ptr, _Count});
    }

    template <class _Elem>
    int string_view<_Elem>::compare(const_pointer _Ptr) const noexcept {
        return compare(string_view{_Ptr});
    }

    template <class _Elem>
    bool string_view<_Elem>::starts_with(const string_view _Str) const noexcept {
        if (_Str._Mysize > _Mysize) {
            return false;
        }

        return traits_type::eq(_Mydata, _Str._Mydata, _Str._Mysize);
    }

    template <class _Elem>
    bool string_view<_Elem>::starts_with(const value_type _Ch) const noexcept {
        return _Mysize > 0 ? _Mydata[0] == _Ch : false;
    }

    template <class _Elem>
    bool string_view<_Elem>::starts_with(const_pointer _Ptr) const noexcept {
        return starts_with(string_view{_Ptr});
    }

    template <class _Elem>
    bool string_view<_Elem>::ends_with(const string_view _Str) const noexcept {
        if (_Str._Mysize > _Mysize) {
            return false;
        }

        return traits_type::eq(_Mydata + (_Mysize - _Str._Mysize), _Str._Mydata, _Str._Mysize);
    }

    template <class _Elem>
    bool string_view<_Elem>::ends_with(const value_type _Ch) const noexcept {
        return _Mysize > 0 ? _Mydata[_Mysize - 1] == _Ch : false;
    }

    template <class _Elem>
    bool string_view<_Elem>::ends_with(const_pointer _Ptr) const noexcept {
        return ends_with(string_view{_Ptr});
    }

    template <class _Elem>
    bool string_view<_Elem>::contains(const string_view _Str) const noexcept {
        return find(_Str) != npos;
    }

    template <class _Elem>
    bool string_view<_Elem>::contains(const value_type _Ch) const noexcept {
        return find(_Ch) != npos;
    }

    template <class _Elem>
    bool string_view<_Elem>::contains(const_pointer _Ptr) const noexcept {
        return find(_Ptr) != npos;
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type
        string_view<_Elem>::find(const string_view _Str, const size_type _Off) const noexcept {
        if (_Off >= _Mysize || _Mysize - _Off < _Str._Mysize) {
            return npos;
        }

        if (_Str._Mysize == 0) { // empty string is always considered as found
            return _Off;
        }
        
        if (_Off == 0) { // offset has no meaning
            return traits_type::find(_Mydata, _Mysize, _Str._Mydata, _Str._Mysize);
        } else {
            const size_type _Idx = traits_type::find(_Mydata + _Off, _Mysize - _Off, _Str._Mydata, _Str._Mysize);
            return _Idx != npos ? _Idx + _Off : npos;
        }
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type
        string_view<_Elem>::find(const value_type _Ch, const size_type _Off) const noexcept {
        if (_Off >= _Mysize) {
            return npos;
        }

        if (_Off == 0) { // offset has no meaning
            return traits_type::find(_Mydata, _Mysize, _Ch);
        } else {
            const size_type _Idx = traits_type::find(_Mydata + _Off, _Mysize - _Off, _Ch);
            return _Idx != npos ? _Idx + _Off : npos;
        }
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type string_view<_Elem>::find(
        const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept {
        if (_Off >= _Mysize) {
            return npos;
        }

        if (_Count == 0) { // empty string is always considered as found
            return _Off;
        }

        if (_Off == 0) { // offset has no meaning
            return traits_type::find(_Mydata, _Mysize, _Ptr, _Count);
        } else {
            const size_type _Idx = traits_type::find(_Mydata + _Off, _Mysize - _Off, _Ptr, _Count);
            return _Idx != npos ? _Idx + _Off : npos;
        }
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type
        string_view<_Elem>::find(const_pointer _Ptr, const size_type _Off) const noexcept {
        return find(string_view{_Ptr}, _Off);
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type
        string_view<_Elem>::rfind(const string_view _Str, const size_type _Off) const noexcept {
        if (_Str._Mysize > _Mysize) {
            return npos;
        }

        if (_Str._Mysize == 0) { // empty string is always considered as found
            return (::std::min)(_Off, _Mysize - 1);
        }

        return traits_type::rfind(
            _Mydata, (::std::min)(_Off, _Mysize - _Str._Mysize), _Str._Mydata, _Str._Mysize);
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type
        string_view<_Elem>::rfind(const value_type _Ch, const size_type _Off) const noexcept {
        if (_Mysize == 0) {
            return npos;
        }

        return traits_type::rfind(_Mydata, (::std::min)(_Off, _Mysize - 1), _Ch);
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type string_view<_Elem>::rfind(
        const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept {
        return rfind(string_view{_Ptr, _Count}, _Off);
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type
        string_view<_Elem>::rfind(const_pointer _Ptr, const size_type _Off) const noexcept {
        return rfind(string_view{_Ptr}, _Off);
    }

    template class _MJSTR_API string_view<byte_t>;
    template class _MJSTR_API string_view<char>;
    template class _MJSTR_API string_view<wchar_t>;
} // namespace mjx