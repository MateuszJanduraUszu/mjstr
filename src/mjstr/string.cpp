// byte_string.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <climits>
#include <mjstr/impl/utils.hpp>
#include <mjstr/string.hpp>
#include <type_traits>

namespace mjx {
    template <class _Elem>
    string_const_iterator<_Elem>::string_const_iterator() noexcept
#ifdef _DEBUG
        : _Myptr(nullptr), _Mybegin(nullptr), _Myend(nullptr) {}
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        : _Myptr(nullptr) {}
#endif // _DEBUG

    template <class _Elem>
    string_const_iterator<_Elem>::string_const_iterator(
#ifdef _DEBUG
        pointer _First, pointer _Last) noexcept : _Myptr(_First), _Mybegin(_First), _Myend(_Last) {
        _INTERNAL_ASSERT(_First <= _Last, "invalid iterator bounds");
    }
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        pointer _Ptr) noexcept : _Myptr(_Ptr) {}
#endif // _DEBUG

    template <class _Elem>
    typename string_const_iterator<_Elem>::reference string_const_iterator<_Elem>::operator*() const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr && _Myptr != _Myend, "attempt to dereference invalid iterator");
#endif // _DEBUG
        return *_Myptr;
    }

    template <class _Elem>
    typename string_const_iterator<_Elem>::pointer string_const_iterator<_Elem>::operator->() const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr && _Myptr != _Myend, "attempt to dereference invalid iterator");
#endif // _DEBUG
        return _Myptr;
    }

    template <class _Elem>
    typename string_const_iterator<_Elem>::reference
        string_const_iterator<_Elem>::operator[](const difference_type _Off) const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myend - _Myptr >= _Off, "attempt to access non-existent element");
#endif // _DEBUG
        return _Myptr[_Off];
    }

    template <class _Elem>
    string_const_iterator<_Elem>& string_const_iterator<_Elem>::operator++() noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myend - _Myptr > 0, "attempt to advance iterator that points to the end");
#endif // _DEBUG
        ++_Myptr;
        return *this;
    }

    template <class _Elem>
    string_const_iterator<_Elem> string_const_iterator<_Elem>::operator++(int) noexcept {
        const string_const_iterator _Temp = *this;
        ++*this;
        return _Temp;
    }

    template <class _Elem>
    string_const_iterator<_Elem>& string_const_iterator<_Elem>::operator--() noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myptr - _Mybegin > 0, "attempt to retreat iterator that points to the beginning");
#endif // _DEBUG
        --_Myptr;
        return *this;
    }

    template <class _Elem>
    string_const_iterator<_Elem> string_const_iterator<_Elem>::operator--(int) noexcept {
        const string_const_iterator _Temp = *this;
        --*this;
        return _Temp;
    }

    template <class _Elem>
    string_const_iterator<_Elem>&
        string_const_iterator<_Elem>::operator+=(const difference_type _Off) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myend - _Myptr >= _Off, "attempt to advance iterator beyond the end");
#endif // _DEBUG
        _Myptr += _Off;
        return *this;
    }

    template <class _Elem>
    string_const_iterator<_Elem>&
        string_const_iterator<_Elem>::operator-=(const difference_type _Off) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myptr - _Mybegin >= _Off, "attempt to retreat iterator beyond the beginning");
#endif // _DEBUG
        _Myptr -= _Off;
        return *this;
    }

    template <class _Elem>
    string_const_iterator<_Elem>
        string_const_iterator<_Elem>::operator+(const difference_type _Off) const noexcept {
        string_const_iterator _Temp = *this;
        _Temp                      += _Off;
        return _Temp;
    }

    template <class _Elem>
    string_const_iterator<_Elem>
        string_const_iterator<_Elem>::operator-(const difference_type _Off) const noexcept {
        string_const_iterator _Temp = *this;
        _Temp                      -= _Off;
        return _Temp;
    }

    template <class _Elem>
    bool string_const_iterator<_Elem>::operator==(const string_const_iterator& _Other) const noexcept {
        return _Myptr == _Other._Myptr;
    }

    template <class _Elem>
    bool string_const_iterator<_Elem>::operator!=(const string_const_iterator& _Other) const noexcept {
        return _Myptr != _Other._Myptr;
    }

    template <class _Elem>
    bool string_const_iterator<_Elem>::operator>(const string_const_iterator& _Other) const noexcept {
        return _Myptr > _Other._Myptr;
    }

    template <class _Elem>
    bool string_const_iterator<_Elem>::operator>=(const string_const_iterator& _Other) const noexcept {
        return _Myptr >= _Other._Myptr;
    }

    template <class _Elem>
    bool string_const_iterator<_Elem>::operator<(const string_const_iterator& _Other) const noexcept {
        return _Myptr < _Other._Myptr;
    }

    template <class _Elem>
    bool string_const_iterator<_Elem>::operator<=(const string_const_iterator& _Other) const noexcept {
        return _Myptr <= _Other._Myptr;
    }

    template _MJSTR_API class string_const_iterator<byte_t>;
    template _MJSTR_API class string_const_iterator<char>;
    template _MJSTR_API class string_const_iterator<wchar_t>;

    template <class _Elem>
    string_iterator<_Elem>::string_iterator() noexcept : _Mybase() {}

    template <class _Elem>
    string_iterator<_Elem>::string_iterator(
#ifdef _DEBUG
        pointer _First, pointer _Last) noexcept : _Mybase(_First, _Last) {}
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        pointer _Ptr) noexcept : _Mybase(_Ptr) {}
#endif // _DEBUG

    template _MJSTR_API class string_iterator<byte_t>;
    template _MJSTR_API class string_iterator<char>;
    template _MJSTR_API class string_iterator<wchar_t>;

    template <class _Elem, class _Traits>
    string<_Elem, _Traits>::string() noexcept : _Mybuf() {}

    template <class _Elem, class _Traits>
    string<_Elem, _Traits>::string(const string& _Other) noexcept : _Mybuf() {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Construct_from_ptr(
            _Other._Mybuf._Get(), _Other._Mybuf._Size), "failed to construct the string");
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        _Construct_from_ptr(_Other._Mybuf._Get(), _Other._Mybuf._Size); // may fail
#endif // _DEBUG
    }

    template <class _Elem, class _Traits>
    string<_Elem, _Traits>::string(string&& _Other) noexcept : _Mybuf() {
        _Take_contents(_Other);
    }

    template <class _Elem, class _Traits>
    string<_Elem, _Traits>::string(const size_type _Count, const value_type _Ch) noexcept : _Mybuf() {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Construct_from_chars(_Count, _Ch), "failed to construct the string");
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        _Construct_from_chars(_Count, _Ch); // may fail
#endif // _DEBUG
    }
    
    template <class _Elem, class _Traits>
    string<_Elem, _Traits>::string(const_pointer _Ptr, const size_type _Count) noexcept : _Mybuf() {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Construct_from_ptr(_Ptr, _Count), "failed to construct the string");
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        _Construct_from_ptr(_Ptr, _Count); // may fail
#endif // _DEBUG
    }

    template <class _Elem, class _Traits>
    string<_Elem, _Traits>::string(const_pointer _Ptr) noexcept : _Mybuf() {
#ifdef _DEBUG
        _INTERNAL_ASSERT(
            _Construct_from_ptr(_Ptr, _Traits::length(_Ptr)), "failed to construct the string");
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        _Construct_from_ptr(_Ptr, _Traits::length(_Ptr)); // may fail
#endif // _DEBUG
    }

    template <class _Elem, class _Traits>
    string<_Elem, _Traits>::string(const string_view<_Elem> _Str) noexcept : _Mybuf() {
#ifdef _DEBUG
        _INTERNAL_ASSERT(
            _Construct_from_ptr(_Str.data(), _Str.size()), "failed to construct the string");
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        _Construct_from_ptr(_Str.data(), _Str.size()); // may fail
#endif // _DEBUG
    }

    template <class _Elem, class _Traits>
    string<_Elem, _Traits>::~string() noexcept {
        _Tidy();
    }

    template <class _Elem, class _Traits>
    string<_Elem, _Traits>::_Internal_buffer::_Internal_buffer() noexcept
        : _Capacity(_Small_buffer_capacity), _Size(0), _Small{0} {}

    template <class _Elem, class _Traits>
    string<_Elem, _Traits>::_Internal_buffer::~_Internal_buffer() noexcept {}

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::_Internal_buffer::_Is_small() const noexcept {
        return _Capacity <= _Small_buffer_capacity;
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::pointer string<_Elem, _Traits>::_Internal_buffer::_Get() noexcept {
        return _Capacity <= _Small_buffer_capacity ? _Small : _Large;
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::const_pointer
        string<_Elem, _Traits>::_Internal_buffer::_Get() const noexcept {
        return _Capacity <= _Small_buffer_capacity ? _Small : _Large;
    }

    template <class _Elem, class _Traits>
    void string<_Elem, _Traits>::_Internal_buffer::_Switch_to_small() noexcept {
        // moves data to small buffer and deallocates large one, assumes the data fits in small buffer
        value_type _Temp[_Small_buffer_size];
        _Traits::copy(_Temp, _Large, _Size);
        mjstr_impl::_Deallocate(_Large, _Capacity + 1);
        _Traits::copy(_Small, _Temp, _Size);
        _Capacity     = _Small_buffer_capacity;
        _Small[_Size] = static_cast<value_type>(0);
    }

    template <class _Elem, class _Traits>
    void string<_Elem, _Traits>::_Internal_buffer::_Swap_small_with_large(_Internal_buffer& _Other) noexcept {
        value_type _Temp[_Small_buffer_size];
        _Traits::copy(_Temp, _Small, _Size);
        _Large = _Other._Large;
        _Traits::copy(_Other._Small, _Temp, _Size);
    }

    template <class _Elem, class _Traits>
    void string<_Elem, _Traits>::_Tidy() noexcept {
        if (_Mybuf._Capacity > _Small_buffer_capacity) { // deallocate heap-allocated buffer
            mjstr_impl::_Deallocate(_Mybuf._Large, _Mybuf._Capacity + 1);
            _Mybuf._Large = nullptr;
        }

        _Mybuf._Capacity = 0;
        _Mybuf._Size     = 0;
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::pointer
        string<_Elem, _Traits>::_Allocate_space_for_capacity(size_type& _Count) noexcept {
        // allocate _Count + 1 elements aligned to _Alloc_align boundary
        _Count |= _Alloc_mask;
        if (_Count > max_size() - 1) { // requested too much memory, break
            return nullptr;
        }

        return mjstr_impl::_Allocate<_Elem>(_Count + 1);
    }

    template <class _Elem, class _Traits>
    void string<_Elem, _Traits>::_Take_contents(string& _Other) noexcept {
        _Tidy(); // destroy the current string
        _Mybuf._Capacity = _Other._Mybuf._Capacity;
        _Mybuf._Size     = _Other._Mybuf._Size;
        if (_Other._Mybuf._Is_small()) { // take small buffer
            // copy data and null-terminator
            _Traits::copy(_Mybuf._Small, _Other._Mybuf._Small, _Other._Mybuf._Size + 1);
            _Other._Mybuf._Small[0] = static_cast<value_type>(0);
        } else { // take large buffer
            _Mybuf._Large           = _Other._Mybuf._Large;
            _Other._Mybuf._Large    = nullptr;
            _Other._Mybuf._Small[0] = static_cast<value_type>(0); // start lifetime of small buffer
        }

        _Other._Mybuf._Capacity = _Small_buffer_capacity;
        _Other._Mybuf._Size     = 0;
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::_Construct_from_ptr(const_pointer _Ptr, const size_type _Count) noexcept {
        if (_Count <= _Small_buffer_capacity) { // use small buffer
            _Traits::copy(_Mybuf._Small, _Ptr, _Count);
            _Mybuf._Size                = _Count;
            _Mybuf._Small[_Mybuf._Size] = static_cast<value_type>(0);
        } else { // use large buffer
            size_type _New_capacity = _Count;
            pointer _New_ptr        = _Allocate_space_for_capacity(_New_capacity);
            if (!_New_ptr) { // allocation failed, break
                return false;
            }

            _Traits::copy(_New_ptr, _Ptr, _Count);
            _Mybuf._Capacity            = _New_capacity;
            _Mybuf._Size                = _Count;
            _Mybuf._Large               = _New_ptr;
            _Mybuf._Large[_Mybuf._Size] = static_cast<value_type>(0);
        }

        return true;
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::_Construct_from_chars(const size_type _Count, const value_type _Ch) noexcept {
        if (_Count <= _Small_buffer_capacity) { // use small buffer
            _Traits::assign(_Mybuf._Small, _Count, _Ch);
            _Mybuf._Size                = _Count;
            _Mybuf._Small[_Mybuf._Size] = static_cast<value_type>(0);
        } else { // use large buffer
            size_type _New_capacity = _Count;
            pointer _New_ptr        = _Allocate_space_for_capacity(_New_capacity);
            if (!_New_ptr) { // allocation failed, break
                return false;
            }

            _Traits::assign(_New_ptr, _Count, _Ch);
            _Mybuf._Capacity            = _New_capacity;
            _Mybuf._Size                = _Count;
            _Mybuf._Large               = _New_ptr;
            _Mybuf._Large[_Mybuf._Size] = static_cast<value_type>(0);
        }

        return true;
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::_Reallocate_assign(const value_type _Ch, const size_type _Count) noexcept {
        size_type _New_capacity = _Count;
        pointer _New_ptr        = _Allocate_space_for_capacity(_New_capacity);
        if (!_New_ptr) { // allocation failed, break
            return false;
        }

        if (!_Mybuf._Is_small()) { // deallocate old buffer
            mjstr_impl::_Deallocate(_Mybuf._Large, _Mybuf._Capacity + 1);
        }

        _Traits::assign(_New_ptr, _Count, _Ch);
        _Mybuf._Capacity            = _New_capacity;
        _Mybuf._Size                = _Count;
        _Mybuf._Large               = _New_ptr;
        _Mybuf._Large[_Mybuf._Size] = static_cast<value_type>(0);
        return true;
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::_Reallocate_assign(const_pointer _Ptr, const size_type _Count) noexcept {
        size_type _New_capacity = _Count;
        pointer _New_ptr        = _Allocate_space_for_capacity(_New_capacity);
        if (!_New_ptr) { // allocation failed, break
            return false;
        }

        if (!_Mybuf._Is_small()) { // deallocate old buffer
            mjstr_impl::_Deallocate(_Mybuf._Large, _Mybuf._Capacity + 1);
        }

        _Traits::copy(_New_ptr, _Ptr, _Count);
        _Mybuf._Capacity            = _New_capacity;
        _Mybuf._Size                = _Count;
        _Mybuf._Large               = _New_ptr;
        _Mybuf._Large[_Mybuf._Size] = static_cast<value_type>(0);
        return true;
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::_Reallocate_insert_back(
        const value_type _Ch, const size_type _Count) noexcept {
        const size_type _New_size = _Mybuf._Size + _Count;
        size_type _New_capacity   = _Mybuf._Capacity + (_New_size - _Mybuf._Capacity);
        pointer _New_ptr          = _Allocate_space_for_capacity(_New_capacity);
        if (!_New_ptr) { // allocation failed, break
            return false;
        }

        _Traits::copy(_New_ptr, _Mybuf._Get(), _Mybuf._Size);
        _Traits::assign(_New_ptr + _Mybuf._Size, _Count, _Ch);
        if (!_Mybuf._Is_small()) { // deallocate old buffer
            mjstr_impl::_Deallocate(_Mybuf._Large, _Mybuf._Capacity + 1);
        }

        _Mybuf._Capacity            = _New_capacity;
        _Mybuf._Size                = _New_size;
        _Mybuf._Large               = _New_ptr;
        _Mybuf._Large[_Mybuf._Size] = static_cast<value_type>(0);
        return true;
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::_Reallocate_insert_back(
        const_pointer _Ptr, const size_type _Count) noexcept {
        const size_type _New_size = _Mybuf._Size + _Count;
        size_type _New_capacity   = _Mybuf._Capacity + (_New_size - _Mybuf._Capacity);
        pointer _New_ptr          = _Allocate_space_for_capacity(_New_capacity);
        if (!_New_ptr) { // allocation failed, break
            return false;
        }

        _Traits::copy(_New_ptr, _Mybuf._Get(), _Mybuf._Capacity);
        _Traits::copy(_New_ptr + _Mybuf._Size, _Ptr, _Count);
        if (!_Mybuf._Is_small()) { // deallocate old buffer
            mjstr_impl::_Deallocate(_Mybuf._Large, _Mybuf._Capacity + 1);
        }

        _Mybuf._Capacity            = _New_capacity;
        _Mybuf._Size                = _New_size;
        _Mybuf._Large               = _New_ptr;
        _Mybuf._Large[_Mybuf._Size] = static_cast<value_type>(0);
        return true;
    }

    template <class _Elem, class _Traits>
    string<_Elem, _Traits>::operator string_view<_Elem, _Traits>() const noexcept {
        return string_view<_Elem, _Traits>{_Mybuf._Get(), _Mybuf._Size};
    }

    template <class _Elem, class _Traits>
    string<_Elem, _Traits>& string<_Elem, _Traits>::operator=(const string& _Str) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(assign(_Str), "failed to assign characters to the string");
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        assign(_Str); // may fail
#endif // _DEBUG
        return *this;
    }

    template <class _Elem, class _Traits>
    string<_Elem, _Traits>& string<_Elem, _Traits>::operator=(string&& _Str) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(assign(::std::move(_Str)), "failed to assign characters to the string");
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        assign(::std::move(_Str)); // may fail
#endif // _DEBUG
        return *this;
    }

    template <class _Elem, class _Traits>
    string<_Elem, _Traits>& string<_Elem, _Traits>::operator=(const_pointer _Ptr) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(assign(_Ptr), "failed to assign characters to the string");
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        assign(_Ptr); // may fail
#endif // _DEBUG
        return *this;
    }

    template <class _Elem, class _Traits>
    string<_Elem, _Traits>& string<_Elem, _Traits>::operator=(const value_type _Ch) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(assign(1, _Ch), "failed to assign a character to the string");
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        assign(1, _Ch); // may fail
#endif // _DEBUG
        return *this;
    }

    template <class _Elem, class _Traits>
    string<_Elem, _Traits>&
        string<_Elem, _Traits>::operator=(const string_view<_Elem, _Traits> _Str) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(assign(_Str), "failed to assign characters to the string");
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        assign(_Str); // may fail
#endif // _DEBUG
        return *this;
    }

    template <class _Elem, class _Traits>
    string<_Elem, _Traits>& string<_Elem, _Traits>::operator+=(const string& _Str) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(append(_Str), "failed to append characters to the string");
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        append(_Str); // may fail
#endif // _DEBUG
        return *this;
    }

    template <class _Elem, class _Traits>
    string<_Elem, _Traits>& string<_Elem, _Traits>::operator+=(const_pointer _Ptr) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(append(_Ptr), "failed to append characters to the string");
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        append(_Ptr); // may fail
#endif // _DEBUG
        return *this;
    }

    template <class _Elem, class _Traits>
    string<_Elem, _Traits>& string<_Elem, _Traits>::operator+=(const value_type _Ch) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(push_back(_Ch), "failed to append character to the string");
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        push_back(_Ch); // may fail
#endif // _DEBUG
        return *this;
    }

    template <class _Elem, class _Traits>
    string<_Elem, _Traits>&
        string<_Elem, _Traits>::operator+=(const string_view<_Elem, _Traits> _Str) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(append(_Str), "failed to append characters to the string");
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        append(_Str); // may fail
#endif // _DEBUG
        return *this;
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::reference
        string<_Elem, _Traits>::operator[](const size_type _Idx) noexcept {
        // no bounds checking is performed, the behavior is undefined if _Idx >= size()
        return _Mybuf._Get()[_Idx];
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::const_reference
        string<_Elem, _Traits>::operator[](const size_type _Idx) const noexcept {
        // no bounds checking is performed, the behavior is undefined if _Idx >= size()
        return _Mybuf._Get()[_Idx];
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::iterator string<_Elem, _Traits>::begin() noexcept {
#ifdef _DEBUG
        return iterator{_Mybuf._Get(), _Mybuf._Get() + _Mybuf._Size};
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        return iterator{_Mybuf._Get()};
#endif // _DEBUG
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::const_iterator string<_Elem, _Traits>::begin() const noexcept {
#ifdef _DEBUG
        return const_iterator{_Mybuf._Get(), _Mybuf._Get() + _Mybuf._Size};
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        return const_iterator{_Mybuf._Get()};
#endif // _DEBUG
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::iterator string<_Elem, _Traits>::end() noexcept {
#ifdef _DEBUG
        return iterator{_Mybuf._Get() + _Mybuf._Size, _Mybuf._Get() + _Mybuf._Size};
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        return iterator{_Mybuf._Get() + _Mybuf._Size};
#endif // _DEBUG
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::const_iterator string<_Elem, _Traits>::end() const noexcept {
#ifdef _DEBUG
        return const_iterator{_Mybuf._Get() + _Mybuf._Size, _Mybuf._Get() + _Mybuf._Size};
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        return const_iterator{_Mybuf._Get() + _Mybuf._Size};
#endif // _DEBUG
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::reference string<_Elem, _Traits>::at(const size_type _Idx) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Idx < _Mybuf._Size, "attempt to access non-existent element");
#endif // _DEBUG
        return _Mybuf._Get()[_Idx];
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::const_reference
        string<_Elem, _Traits>::at(const size_type _Idx) const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Idx < _Mybuf._Size, "attempt to access non-existent element");
#endif // _DEBUG
        return _Mybuf._Get()[_Idx];
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::reference string<_Elem, _Traits>::front() noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Mybuf._Size > 0, "attempt to access non-existent element");
#endif // _DEBUG
        return *_Mybuf._Get();
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::const_reference string<_Elem, _Traits>::front() const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Mybuf._Size > 0, "attempt to access non-existent element");
#endif // _DEBUG
        return *_Mybuf._Get();
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::reference string<_Elem, _Traits>::back() noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Mybuf._Size > 0, "attempt to access non-existent element");
#endif // _DEBUG
        return _Mybuf._Get()[_Mybuf._Size - 1];
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::const_reference string<_Elem, _Traits>::back() const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Mybuf._Size > 0, "attempt to access non-existent element");
#endif // _DEBUG
        return _Mybuf._Get()[_Mybuf._Size - 1];
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::pointer string<_Elem, _Traits>::data() noexcept {
        return _Mybuf._Get();
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::const_pointer string<_Elem, _Traits>::data() const noexcept {
        return _Mybuf._Get();
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::const_pointer string<_Elem, _Traits>::c_str() const noexcept {
        return _Mybuf._Get();
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::empty() const noexcept {
        return _Mybuf._Size == 0;
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::size_type string<_Elem, _Traits>::capacity() const noexcept {
        return _Mybuf._Capacity;
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::size_type string<_Elem, _Traits>::size() const noexcept {
        return _Mybuf._Size;
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::size_type string<_Elem, _Traits>::max_size() noexcept {
        return mjstr_impl::_Min(static_cast<size_type>(PTRDIFF_MAX),
            static_cast<size_type>(-1) / sizeof(value_type)) - 1;
    }

    template <class _Elem, class _Traits>
    string_view<_Elem, _Traits> string<_Elem, _Traits>::view() const noexcept {
        return string_view<_Elem, _Traits>{_Mybuf._Get(), _Mybuf._Size};
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::reserve(size_type _New_capacity) noexcept {
        if (_Mybuf._Capacity >= _New_capacity || _New_capacity <= _Small_buffer_capacity) {
            // already reserved requested or more capacity, or fits in small buffer, do nothing
            return true;
        }

        pointer _New_ptr = _Allocate_space_for_capacity(_New_capacity);
        if (!_New_ptr) { // allocation failed, break
            return false;
        }

        _Traits::copy(_New_ptr, _Mybuf._Get(), _Mybuf._Size + 1); // copy data and null-terminator
        if (!_Mybuf._Is_small()) { // deallocate old buffer
            mjstr_impl::_Deallocate(_Mybuf._Large, _Mybuf._Capacity + 1);
        }

        _Mybuf._Capacity = _New_capacity;
        _Mybuf._Large    = _New_ptr;
        return true;
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::size_type string<_Elem, _Traits>::copy(
        pointer _Dest, size_type _Count, const size_type _Off) const noexcept {
        return view().copy(_Dest, _Count, _Off);
    }

    template <class _Elem, class _Traits>
    void string<_Elem, _Traits>::swap(string& _Other) noexcept {
        if (this != ::std::addressof(_Other)) {
            const bool _This_small  = _Mybuf._Is_small();
            const bool _Other_small = _Other._Mybuf._Is_small();
            if (_This_small && _Other_small) { // swap two small buffers
                // copy null-terminators to skip assigning them after copying
                value_type _Temp[_Small_buffer_size];
                _Traits::copy(_Temp, _Mybuf._Small, _Mybuf._Size + 1);
                _Traits::copy(_Mybuf._Small, _Other._Mybuf._Small, _Other._Mybuf._Size + 1);
                _Traits::copy(_Other._Mybuf._Small, _Temp, _Mybuf._Size + 1);
            } else if (_This_small) { // swap small with large
                _Mybuf._Swap_small_with_large(_Other._Mybuf);
            } else if (_Other_small) { // swap large with small
                _Other._Mybuf._Swap_small_with_large(_Mybuf);
            } else { // swap two large buffers
                mjstr_impl::_Swap(_Mybuf._Large, _Other._Mybuf._Large);
            }

            mjstr_impl::_Swap(_Mybuf._Size, _Other._Mybuf._Size);
            mjstr_impl::_Swap(_Mybuf._Capacity, _Other._Mybuf._Capacity);
        }
    }

    template <class _Elem, class _Traits>
    void string<_Elem, _Traits>::clear() noexcept {
        if (_Mybuf._Size > 0) {
            *_Mybuf._Get() = static_cast<value_type>(0);
            _Mybuf._Size   = 0;
        }
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::resize(const size_type _New_size, const value_type _Ch) noexcept {
        if (_New_size <= _Mybuf._Size) { // decrease buffer size
            _Mybuf._Size                = _New_size;
            _Mybuf._Get()[_Mybuf._Size] = static_cast<value_type>(0);
            return true;
        } else { // increase buffer size
            return append(_New_size - _Mybuf._Size, _Ch);
        }
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::expand(const size_type _Count, const value_type _Ch) noexcept {
        if (_Count == 0) { // no expanding, do nothing
            return true;
        }

        if (_Mybuf._Size + _Count <= _Mybuf._Capacity) { // found enough space, don't reallocate memory
            pointer _Buf_ptr = _Mybuf._Get();
            _Traits::assign(_Buf_ptr + _Mybuf._Size, _Count, _Ch);
            _Mybuf._Size          += _Count;
            _Buf_ptr[_Mybuf._Size] = static_cast<value_type>(0);
            return true;
        }

        return _Reallocate_insert_back(_Ch, _Count);
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::shrink(const size_type _Count) noexcept {
        if (_Count == 0) { // no shrinking, do nothing
            return true;
        }

        if (_Count > _Mybuf._Size) { // not enough characters to shrink, break
            return false;
        }

        _Mybuf._Size               -= _Count;
        _Mybuf._Get()[_Mybuf._Size] = static_cast<value_type>(0);
        return true;
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::shrink_to_fit() noexcept {
        if (_Mybuf._Is_small()) { // small string is always considered as fit
            return true;
        }

        if (_Mybuf._Size <= _Small_buffer_capacity) { // switch to small buffer
            _Mybuf._Switch_to_small();
            return true;
        }

        size_type _New_capacity = _Mybuf._Size | _Alloc_mask;
        if (_New_capacity < _Mybuf._Capacity) { // worth shrinking, do it
            pointer _New_ptr = _Allocate_space_for_capacity(_New_capacity);
            if (!_New_ptr) { // allocation failed, break
                return false;
            }

            _Traits::copy(_New_ptr, _Mybuf._Large, _Mybuf._Size);
            mjstr_impl::_Deallocate(_Mybuf._Large, _Mybuf._Capacity + 1);
            _Mybuf._Capacity            = _New_capacity;
            _Mybuf._Large               = _New_ptr;
            _Mybuf._Large[_Mybuf._Size] = static_cast<value_type>(0);
        }

        return true;
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::push_back(const value_type _Ch) noexcept {
        if (_Mybuf._Size + 1 <= _Mybuf._Capacity) { // found enough space, don't reallocate memory
            pointer _Buf_ptr         = _Mybuf._Get();
            _Buf_ptr[_Mybuf._Size]   = _Ch;
            _Buf_ptr[++_Mybuf._Size] = static_cast<value_type>(0);
            return true;
        }

        return _Reallocate_insert_back(_Ch, 1);
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::pop_back() noexcept {
        if (_Mybuf._Size == 0) { // not enough characters
            return false;
        }

        --_Mybuf._Size;
        _Mybuf._Get()[_Mybuf._Size] = static_cast<value_type>(0);
        return true;
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::assign(const size_type _Count, const value_type _Ch) noexcept {
        if (_Count <= _Mybuf._Capacity) { // found enough space, don't reallocate memory
            pointer _Buf_ptr = _Mybuf._Get();
            _Traits::assign(_Buf_ptr, _Count, _Ch);
            _Mybuf._Size           = _Count;
            _Buf_ptr[_Mybuf._Size] = static_cast<value_type>(0);
            return true;
        }

        return _Reallocate_assign(_Ch, _Count);
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::assign(const string& _Str) noexcept {
        if (this == ::std::addressof(_Str)) { // must not be this string
            return false;
        }

        return assign(_Str._Mybuf._Get(), _Str._Mybuf._Size);
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::assign(string&& _Str) noexcept {
        if (this == ::std::addressof(_Str)) { // must not be this string
            return false;
        }

        _Take_contents(_Str);
        return true;
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::assign(const_pointer _Ptr, const size_type _Count) noexcept {
        if (_Count <= _Mybuf._Capacity) { // found enough space, don't reallocate memory
            pointer _Buf_ptr = _Mybuf._Get();
            _Traits::copy(_Buf_ptr, _Ptr, _Count);
            _Mybuf._Size           = _Count;
            _Buf_ptr[_Mybuf._Size] = static_cast<value_type>(0);
            return true;
        }

        return _Reallocate_assign(_Ptr, _Count);
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::assign(const_pointer _Ptr) noexcept {
        return assign(_Ptr, _Traits::length(_Ptr));
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::assign(const string_view<_Elem, _Traits> _Str) noexcept {
        return assign(_Str.data(), _Str.size());
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::append(const size_type _Count, const value_type _Ch) noexcept {
        if (_Mybuf._Size + _Count <= _Mybuf._Capacity) { // found enough space, don't reallocate memory
            pointer _Buf_ptr = _Mybuf._Get();
            _Traits::assign(_Buf_ptr + _Mybuf._Size, _Count, _Ch);
            _Mybuf._Size          += _Count;
            _Buf_ptr[_Mybuf._Size] = static_cast<value_type>(0);
            return true;
        }

        return _Reallocate_insert_back(_Ch, _Count);
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::append(const string& _Str) noexcept {
        return append(_Str._Mybuf._Get(), _Str._Mybuf._Size);
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::append(const_pointer _Ptr, const size_type _Count) noexcept {
        if (_Mybuf._Size + _Count <= _Mybuf._Capacity) { // found enough space, don't reallocate memory
            pointer _Buf_ptr = _Mybuf._Get();
            _Traits::copy(_Buf_ptr + _Mybuf._Size, _Ptr, _Count);
            _Mybuf._Size          += _Count;
            _Buf_ptr[_Mybuf._Size] = static_cast<value_type>(0);
            return true;
        }

        return _Reallocate_insert_back(_Ptr, _Count);
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::append(const_pointer _Ptr) noexcept {
        return append(_Ptr, _Traits::length(_Ptr));
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::append(const string_view<_Elem, _Traits> _Str) noexcept {
        return append(_Str.data(), _Str.size());
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::size_type
        string<_Elem, _Traits>::find(const string& _Str, const size_type _Off) const noexcept {
        return view().find(_Str.view(), _Off);
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::size_type string<_Elem, _Traits>::find(
        const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept {
        return view().find(_Ptr, _Off, _Count);
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::size_type string<_Elem, _Traits>::find(
        const value_type _Ch, const size_type _Off) const noexcept {
        return view().find(_Ch, _Off);
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::size_type string<_Elem, _Traits>::find(
        const string_view<_Elem, _Traits> _Str, const size_type _Off) const noexcept {
        return view().find(_Str, _Off);
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::size_type
        string<_Elem, _Traits>::rfind(const string& _Str, const size_type _Off) const noexcept {
        return view().rfind(_Str.view(), _Off);
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::size_type string<_Elem, _Traits>::rfind(
        const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept {
        return view().rfind(_Ptr, _Off, _Count);
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::size_type string<_Elem, _Traits>::rfind(
        const_pointer _Ptr, const size_type _Off) const noexcept {
        return view().rfind(_Ptr, _Off);
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::size_type string<_Elem, _Traits>::rfind(
        const value_type _Ch, const size_type _Off) const noexcept {
        return view().rfind(_Ch, _Off);
    }

    template <class _Elem, class _Traits>
    typename string<_Elem, _Traits>::size_type string<_Elem, _Traits>::rfind(
        const string_view<_Elem, _Traits> _Str, const size_type _Off) const noexcept {
        return view().rfind(_Str, _Off);
    }

    template <class _Elem, class _Traits>
    int string<_Elem, _Traits>::compare(const string& _Str) const noexcept {
        return _Traits::compare(
            _Mybuf._Get(), _Mybuf._Size, _Str._Mybuf._Get(), _Str._Mybuf._Size);
    }

    template <class _Elem, class _Traits>
    int string<_Elem, _Traits>::compare(const_pointer _Ptr, const size_type _Count) const noexcept {
        return _Traits::compare(_Mybuf._Get(), _Mybuf._Size, _Ptr, _Count);
    }

    template <class _Elem, class _Traits>
    int string<_Elem, _Traits>::compare(const_pointer _Ptr) const noexcept {
        return _Traits::compare(_Mybuf._Get(), _Mybuf._Size, _Ptr, _Traits::length(_Ptr));
    }

    template <class _Elem, class _Traits>
    int string<_Elem, _Traits>::compare(const string_view<_Elem, _Traits> _Str) const noexcept {
        return _Traits::compare(_Mybuf._Get(), _Mybuf._Size, _Str.data(), _Str.size());
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::starts_with(const string_view<_Elem, _Traits> _Str) const noexcept {
        return view().starts_with(_Str);
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::starts_with(const value_type _Ch) const noexcept {
        return view().starts_with(_Ch);
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::starts_with(const_pointer _Ptr) const noexcept {
        return view().starts_with(_Ptr);
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::ends_with(const string_view<_Elem, _Traits> _Str) const noexcept {
        return view().ends_with(_Str);
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::ends_with(const value_type _Ch) const noexcept {
        return view().ends_with(_Ch);
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::ends_with(const_pointer _Ptr) const noexcept {
        return view().ends_with(_Ptr);
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::contains(const string_view<_Elem, _Traits> _Str) const noexcept {
        return find(_Str) != npos;
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::contains(const value_type _Ch) const noexcept {
        return find(_Ch) != npos;
    }

    template <class _Elem, class _Traits>
    bool string<_Elem, _Traits>::contains(const_pointer _Ptr) const noexcept {
        return find(string_view<_Elem>{_Ptr, _Traits::length(_Ptr)}) != npos;
    }

    template <class _Elem, class _Traits>
    string<_Elem, _Traits>
        string<_Elem, _Traits>::substr(const size_type _Off, size_type _Count) const noexcept {
        if (_Off >= _Mybuf._Size) {
            return string{};
        }

        _Count = mjstr_impl::_Min(_Count, _Mybuf._Size - _Off); // trim number of characters
        return string{_Mybuf._Get() + _Off, _Count};
    }

    template _MJSTR_API class string<byte_t, char_traits<byte_t>>;
    template _MJSTR_API class string<char, char_traits<char>>;
    template _MJSTR_API class string<wchar_t, char_traits<wchar_t>>;
} // namespace mjx