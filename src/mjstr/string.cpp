// byte_string.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <algorithm>
#include <cstdint>
#include <mjmem/exception.hpp>
#include <mjmem/object_allocator.hpp>
#include <mjstr/impl/utils.hpp>
#include <mjstr/string.hpp>

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
    ::std::strong_ordering
        string_const_iterator<_Elem>::operator<=>(const string_const_iterator& _Other) const noexcept {
        return _Myptr <=> _Other._Myptr;
    }

    template class _MJSTR_API string_const_iterator<byte_t>;
    template class _MJSTR_API string_const_iterator<char>;
    template class _MJSTR_API string_const_iterator<wchar_t>;

    template <class _Elem>
    string_iterator<_Elem>::string_iterator() noexcept : _Mybase() {}

    template <class _Elem>
    string_iterator<_Elem>::string_iterator(
#ifdef _DEBUG
        pointer _First, pointer _Last) noexcept : _Mybase(_First, _Last) {}
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        pointer _Ptr) noexcept : _Mybase(_Ptr) {}
#endif // _DEBUG

    template <class _Elem>
    typename string_iterator<_Elem>::reference string_iterator<_Elem>::operator*() const noexcept {
        return const_cast<reference>(_Mybase::operator*());
    }

    template <class _Elem>
    typename string_iterator<_Elem>::pointer string_iterator<_Elem>::operator->() const noexcept {
        return const_cast<pointer>(_Mybase::operator->());
    }

    template <class _Elem>
    typename string_iterator<_Elem>::reference
        string_iterator<_Elem>::operator[](const difference_type _Off) const noexcept {
        return const_cast<reference>(_Mybase::operator[](_Off));
    }

    template <class _Elem>
    string_iterator<_Elem>& string_iterator<_Elem>::operator++() noexcept {
        _Mybase::operator++();
        return *this;
    }

    template <class _Elem>
    string_iterator<_Elem> string_iterator<_Elem>::operator++(int) noexcept {
        string_iterator _Temp = *this;
        _Mybase::operator++();
        return _Temp;
    }

    template <class _Elem>
    string_iterator<_Elem>& string_iterator<_Elem>::operator--() noexcept {
        _Mybase::operator--();
        return *this;
    }

    template <class _Elem>
    string_iterator<_Elem> string_iterator<_Elem>::operator--(int) noexcept {
        string_iterator _Temp = *this;
        _Mybase::operator--();
        return _Temp;
    }

    template <class _Elem>
    string_iterator<_Elem>& string_iterator<_Elem>::operator+=(const difference_type _Off) noexcept {
        _Mybase::operator+=(_Off);
        return *this;
    }

    template <class _Elem>
    string_iterator<_Elem>& string_iterator<_Elem>::operator-=(const difference_type _Off) noexcept {
        _Mybase::operator-=(_Off);
        return *this;
    }

    template <class _Elem>
    string_iterator<_Elem> string_iterator<_Elem>::operator+(const difference_type _Off) const noexcept {
        string_iterator _Temp = *this;
        _Temp                += _Off;
        return _Temp;
    }

    template <class _Elem>
    string_iterator<_Elem> string_iterator<_Elem>::operator-(const difference_type _Off) const noexcept {
        string_iterator _Temp = *this;
        _Temp                -= _Off;
        return _Temp;
    }

    template class _MJSTR_API string_iterator<byte_t>;
    template class _MJSTR_API string_iterator<char>;
    template class _MJSTR_API string_iterator<wchar_t>;

    template <class _Elem>
    string<_Elem>::string() noexcept : _Mybuf() {}

    template <class _Elem>
    string<_Elem>::string(const string& _Other) : _Mybuf() {
        _Construct_from_ptr(_Other._Mybuf._Get(), _Other._Mybuf._Size);
    }

    template <class _Elem>
    string<_Elem>::string(string&& _Other) noexcept : _Mybuf() {
        _Take_contents(_Other);
    }

    template <class _Elem>
    string<_Elem>::string(const size_type _Count, const value_type _Ch) : _Mybuf() {
        _Construct_from_chars(_Count, _Ch);
    }
    
    template <class _Elem>
    string<_Elem>::string(const_pointer _Ptr, const size_type _Count) : _Mybuf() {
        _Construct_from_ptr(_Ptr, _Count);
    }

    template <class _Elem>
    string<_Elem>::string(const_pointer _Ptr) : _Mybuf() {
        _Construct_from_ptr(_Ptr, traits_type::length(_Ptr));
    }

    template <class _Elem>
    string<_Elem>::string(const string_view<_Elem> _Str) : _Mybuf() {
        _Construct_from_ptr(_Str.data(), _Str.size());
    }

    template <class _Elem>
    string<_Elem>::~string() noexcept {
        _Tidy();
    }

    template <class _Elem>
    string<_Elem>::_Internal_buffer::_Internal_buffer() noexcept
        : _Capacity(_Small_buffer_capacity), _Size(0), _Small{0} {}

    template <class _Elem>
    string<_Elem>::_Internal_buffer::~_Internal_buffer() noexcept {}

    template <class _Elem>
    bool string<_Elem>::_Internal_buffer::_Is_small() const noexcept {
        return _Capacity <= _Small_buffer_capacity;
    }

    template <class _Elem>
    typename string<_Elem>::pointer string<_Elem>::_Internal_buffer::_Get() noexcept {
        return _Capacity <= _Small_buffer_capacity ? _Small : _Large;
    }

    template <class _Elem>
    typename string<_Elem>::const_pointer string<_Elem>::_Internal_buffer::_Get() const noexcept {
        return _Capacity <= _Small_buffer_capacity ? _Small : _Large;
    }

    template <class _Elem>
    void string<_Elem>::_Internal_buffer::_Switch_to_small() noexcept {
        // moves data to small buffer and deallocates large one, assumes the data fits in small buffer
        value_type _Temp[_Small_buffer_size];
        traits_type::copy(_Temp, _Large, _Size);
        ::mjx::delete_object_array(_Large, _Capacity + 1);
        traits_type::copy(_Small, _Temp, _Size);
        _Capacity     = _Small_buffer_capacity;
        _Small[_Size] = static_cast<value_type>(0);
    }

    template <class _Elem>
    void string<_Elem>::_Internal_buffer::_Swap_small_with_large(_Internal_buffer& _Other) noexcept {
        value_type _Temp[_Small_buffer_size];
        traits_type::copy(_Temp, _Small, _Size);
        _Large = _Other._Large;
        traits_type::copy(_Other._Small, _Temp, _Size);
    }

    template <class _Elem>
    void string<_Elem>::_Internal_buffer::_Deallocate_if_large() noexcept {
        if (_Capacity > _Small_buffer_capacity) {
            ::mjx::delete_object_array(_Large, _Capacity + 1);
            _Large = nullptr;
        }
    }

    template <class _Elem>
    typename string<_Elem>::pointer string<_Elem>::_Allocate_space_for_capacity(size_type& _Count) {
        // allocate _Count + 1 elements aligned to _Alloc_align boundary
        _Count |= _Alloc_mask;
        if (_Count > max_size() - 1) { // requested too much memory, break
            allocation_limit_exceeded::raise();
        }

        return ::mjx::allocate_object_array<_Elem>(_Count + 1);
    }

    template <class _Elem>
    void string<_Elem>::_Tidy() noexcept {
        _Mybuf._Deallocate_if_large();
        _Mybuf._Capacity = 0;
        _Mybuf._Size     = 0;
    }

    template <class _Elem>
    void string<_Elem>::_Check_offset(const size_type _Off) const {
        if (_Off >= _Mybuf._Size) { // must be within [0, _Mybuf._Size)
            resource_overrun::raise();
        }
    }

    template <class _Elem>
    void string<_Elem>::_Check_offset_for_insertion(const size_type _Off) const {
        if (_Off > _Mybuf._Size) { // must be within [0, _Mybuf._Size]
            resource_overrun::raise();
        }
    }

    template <class _Elem>
    void string<_Elem>::_Take_contents(string& _Other) noexcept {
        _Tidy(); // destroy the current string
        _Mybuf._Capacity = _Other._Mybuf._Capacity;
        _Mybuf._Size     = _Other._Mybuf._Size;
        if (_Other._Mybuf._Is_small()) { // take small buffer
            // copy data and null-terminator
            traits_type::copy(_Mybuf._Small, _Other._Mybuf._Small, _Other._Mybuf._Size + 1);
            _Other._Mybuf._Small[0] = static_cast<value_type>(0);
        } else { // take large buffer
            _Mybuf._Large           = _Other._Mybuf._Large;
            _Other._Mybuf._Large    = nullptr;
            _Other._Mybuf._Small[0] = static_cast<value_type>(0); // start lifetime of small buffer
        }

        _Other._Mybuf._Capacity = _Small_buffer_capacity;
        _Other._Mybuf._Size     = 0;
    }

    template <class _Elem>
    void string<_Elem>::_Construct_from_ptr(const_pointer _Ptr, const size_type _Count) {
        if (_Count <= _Small_buffer_capacity) { // use small buffer
            traits_type::copy(_Mybuf._Small, _Ptr, _Count);
            _Mybuf._Size                = _Count;
            _Mybuf._Small[_Mybuf._Size] = static_cast<value_type>(0);
        } else { // use large buffer
            size_type _New_capacity = _Count;
            pointer _New_ptr        = _Allocate_space_for_capacity(_New_capacity); // may throw
            traits_type::copy(_New_ptr, _Ptr, _Count);
            _Mybuf._Capacity            = _New_capacity;
            _Mybuf._Size                = _Count;
            _Mybuf._Large               = _New_ptr;
            _Mybuf._Large[_Mybuf._Size] = static_cast<value_type>(0);
        }
    }

    template <class _Elem>
    void string<_Elem>::_Construct_from_chars(const size_type _Count, const value_type _Ch) {
        if (_Count <= _Small_buffer_capacity) { // use small buffer
            traits_type::assign(_Mybuf._Small, _Count, _Ch);
            _Mybuf._Size                = _Count;
            _Mybuf._Small[_Mybuf._Size] = static_cast<value_type>(0);
        } else { // use large buffer
            size_type _New_capacity = _Count;
            pointer _New_ptr        = _Allocate_space_for_capacity(_New_capacity); // may throw
            traits_type::assign(_New_ptr, _Count, _Ch);
            _Mybuf._Capacity            = _New_capacity;
            _Mybuf._Size                = _Count;
            _Mybuf._Large               = _New_ptr;
            _Mybuf._Large[_Mybuf._Size] = static_cast<value_type>(0);
        }
    }

    template <class _Elem>
    void string<_Elem>::_Reallocate_assign(const size_type _Count, const value_type _Ch) {
        size_type _New_capacity = _Count;
        pointer _New_ptr        = _Allocate_space_for_capacity(_New_capacity); // may throw
        _Mybuf._Deallocate_if_large();
        traits_type::assign(_New_ptr, _Count, _Ch);
        _Mybuf._Capacity            = _New_capacity;
        _Mybuf._Size                = _Count;
        _Mybuf._Large               = _New_ptr;
        _Mybuf._Large[_Mybuf._Size] = static_cast<value_type>(0);
    }

    template <class _Elem>
    void string<_Elem>::_Reallocate_assign(const_pointer _Ptr, const size_type _Count) {
        size_type _New_capacity = _Count;
        pointer _New_ptr        = _Allocate_space_for_capacity(_New_capacity); // may throw
        _Mybuf._Deallocate_if_large();
        traits_type::copy(_New_ptr, _Ptr, _Count);
        _Mybuf._Capacity            = _New_capacity;
        _Mybuf._Size                = _Count;
        _Mybuf._Large               = _New_ptr;
        _Mybuf._Large[_Mybuf._Size] = static_cast<value_type>(0);
    }

    template <class _Elem>
    void string<_Elem>::_Reallocate_insert_back(const size_type _Count, const value_type _Ch) {
        const size_type _New_size = _Mybuf._Size + _Count;
        size_type _New_capacity   = _Mybuf._Capacity + (_New_size - _Mybuf._Capacity);
        pointer _New_ptr          = _Allocate_space_for_capacity(_New_capacity); // may throw
        traits_type::copy(_New_ptr, _Mybuf._Get(), _Mybuf._Size);
        traits_type::assign(_New_ptr + _Mybuf._Size, _Count, _Ch);
        _Mybuf._Deallocate_if_large();
        _Mybuf._Capacity            = _New_capacity;
        _Mybuf._Size                = _New_size;
        _Mybuf._Large               = _New_ptr;
        _Mybuf._Large[_Mybuf._Size] = static_cast<value_type>(0);
    }

    template <class _Elem>
    void string<_Elem>::_Reallocate_insert_back(const_pointer _Ptr, const size_type _Count) {
        const size_type _New_size = _Mybuf._Size + _Count;
        size_type _New_capacity   = _Mybuf._Capacity + (_New_size - _Mybuf._Capacity);
        pointer _New_ptr          = _Allocate_space_for_capacity(_New_capacity); // may throw
        traits_type::copy(_New_ptr, _Mybuf._Get(), _Mybuf._Capacity);
        traits_type::copy(_New_ptr + _Mybuf._Size, _Ptr, _Count);
        _Mybuf._Deallocate_if_large();
        _Mybuf._Capacity            = _New_capacity;
        _Mybuf._Size                = _New_size;
        _Mybuf._Large               = _New_ptr;
        _Mybuf._Large[_Mybuf._Size] = static_cast<value_type>(0);
    }

    template <class _Elem>
    void string<_Elem>::_Reallocate_insert_at(const size_type _Off, const size_type _Count, const value_type _Ch) {
        const size_type _New_size = _Mybuf._Size + _Count;
        size_type _New_capacity   = _Mybuf._Capacity + (_New_size - _Mybuf._Capacity);
        pointer _New_ptr          = _Allocate_space_for_capacity(_New_capacity); // may throw
        const_pointer _Old_ptr    = _Mybuf._Get();
        traits_type::copy(_New_ptr, _Old_ptr, _Off);
        traits_type::assign(_New_ptr + _Off, _Count, _Ch);
        traits_type::copy(_New_ptr + _Off + _Count, _Old_ptr + _Off, _Mybuf._Size - _Off + 1);
        _Mybuf._Deallocate_if_large();
        _Mybuf._Capacity = _New_capacity;
        _Mybuf._Size     = _New_size;
        _Mybuf._Large    = _New_ptr;
    }

    template <class _Elem>
    void string<_Elem>::_Reallocate_insert_at(const size_type _Off, const_pointer _Ptr, const size_type _Count) {
        const size_type _New_size = _Mybuf._Size + _Count;
        size_type _New_capacity   = _Mybuf._Capacity + (_New_size - _Mybuf._Capacity);
        pointer _New_ptr          = _Allocate_space_for_capacity(_New_capacity); // may throw
        const_pointer _Old_ptr    = _Mybuf._Get();
        traits_type::copy(_New_ptr, _Old_ptr, _Off);
        traits_type::copy(_New_ptr + _Off, _Ptr, _Count);
        traits_type::copy(_New_ptr + _Off + _Count, _Old_ptr + _Off, _Mybuf._Size - _Off + 1);
        _Mybuf._Deallocate_if_large();
        _Mybuf._Capacity = _New_capacity;
        _Mybuf._Size     = _New_size;
        _Mybuf._Large    = _New_ptr;
    }

    template <class _Elem>
    void string<_Elem>::_Reallocate_replace(
        const size_type _Off, const size_type _Count, const size_type _Ch_count, const value_type _Ch) {
        const size_type _New_size = _Mybuf._Size + (_Ch_count - _Count);
        size_type _New_capacity   = _Mybuf._Capacity + (_New_size - _Mybuf._Capacity);
        pointer _New_ptr          = _Allocate_space_for_capacity(_New_capacity); // may throw
        const_pointer _Old_ptr    = _Mybuf._Get();
        traits_type::copy(_New_ptr, _Old_ptr, _Off);
        traits_type::assign(_New_ptr + _Off, _Ch_count, _Ch);
        traits_type::copy(
            _New_ptr + _Off + _Ch_count, _Old_ptr + _Off + _Count, _Mybuf._Size - _Off - _Count + 1);
        _Mybuf._Deallocate_if_large();
        _Mybuf._Capacity = _New_capacity;
        _Mybuf._Size     = _New_size;
        _Mybuf._Large    = _New_ptr;
    }

    template <class _Elem>
    void string<_Elem>::_Reallocate_replace(
        const size_type _Off, const size_type _Count, const_pointer _Ptr, const size_type _Ptr_count) {
        const size_type _New_size = _Mybuf._Size + (_Ptr_count - _Count);
        size_type _New_capacity   = _Mybuf._Capacity + (_New_size - _Mybuf._Capacity);
        pointer _New_ptr          = _Allocate_space_for_capacity(_New_capacity); // may throw
        const_pointer _Old_ptr    = _Mybuf._Get();
        traits_type::copy(_New_ptr, _Old_ptr, _Off);
        traits_type::copy(_New_ptr + _Off, _Ptr, _Ptr_count);
        traits_type::copy(
            _New_ptr + _Off + _Ptr_count, _Old_ptr + _Off + _Count, _Mybuf._Size - _Off - _Count + 1);
        _Mybuf._Deallocate_if_large();
        _Mybuf._Capacity = _New_capacity;
        _Mybuf._Size     = _New_size;
        _Mybuf._Large    = _New_ptr;
    }

    template <class _Elem>
    string<_Elem>::operator string_view<_Elem>() const noexcept {
        return string_view<_Elem>{_Mybuf._Get(), _Mybuf._Size};
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::operator=(const string& _Str) {
        return assign(_Str._Mybuf._Get(), _Str._Mybuf._Size);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::operator=(string&& _Str) noexcept {
        return assign(::std::move(_Str));
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::operator=(const_pointer _Ptr) {
        return assign(_Ptr, traits_type::length(_Ptr));
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::operator=(const value_type _Ch) {
        return assign(1, _Ch);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::operator=(const string_view<_Elem> _Str) {
        return assign(_Str.data(), _Str.size());
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::operator+=(const string& _Str) {
        return append(_Str._Mybuf._Get(), _Str._Mybuf._Size);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::operator+=(const_pointer _Ptr) {
        return append(_Ptr, traits_type::length(_Ptr));
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::operator+=(const value_type _Ch) {
        push_back(_Ch);
        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::operator+=(const string_view<_Elem> _Str) {
        return append(_Str.data(), _Str.size());
    }

    template <class _Elem>
    typename string<_Elem>::reference string<_Elem>::operator[](const size_type _Idx) noexcept {
        // no bounds checking is performed, the behavior is undefined if _Idx >= size()
        return _Mybuf._Get()[_Idx];
    }

    template <class _Elem>
    typename string<_Elem>::const_reference string<_Elem>::operator[](const size_type _Idx) const noexcept {
        // no bounds checking is performed, the behavior is undefined if _Idx >= size()
        return _Mybuf._Get()[_Idx];
    }

    template <class _Elem>
    typename string<_Elem>::iterator string<_Elem>::begin() noexcept {
#ifdef _DEBUG
        return iterator{_Mybuf._Get(), _Mybuf._Get() + _Mybuf._Size};
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        return iterator{_Mybuf._Get()};
#endif // _DEBUG
    }

    template <class _Elem>
    typename string<_Elem>::const_iterator string<_Elem>::begin() const noexcept {
#ifdef _DEBUG
        return const_iterator{_Mybuf._Get(), _Mybuf._Get() + _Mybuf._Size};
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        return const_iterator{_Mybuf._Get()};
#endif // _DEBUG
    }

    template <class _Elem>
    typename string<_Elem>::iterator string<_Elem>::end() noexcept {
#ifdef _DEBUG
        return iterator{_Mybuf._Get() + _Mybuf._Size, _Mybuf._Get() + _Mybuf._Size};
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        return iterator{_Mybuf._Get() + _Mybuf._Size};
#endif // _DEBUG
    }

    template <class _Elem>
    typename string<_Elem>::const_iterator string<_Elem>::end() const noexcept {
#ifdef _DEBUG
        return const_iterator{_Mybuf._Get() + _Mybuf._Size, _Mybuf._Get() + _Mybuf._Size};
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        return const_iterator{_Mybuf._Get() + _Mybuf._Size};
#endif // _DEBUG
    }

    template <class _Elem>
    typename string<_Elem>::reference string<_Elem>::at(const size_type _Idx) {
        _Check_offset(_Idx);
        return _Mybuf._Get()[_Idx];
    }

    template <class _Elem>
    typename string<_Elem>::const_reference string<_Elem>::at(const size_type _Idx) const {
        _Check_offset(_Idx);
        return _Mybuf._Get()[_Idx];
    }

    template <class _Elem>
    typename string<_Elem>::reference string<_Elem>::front() noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Mybuf._Size > 0, "attempt to access non-existent element");
#endif // _DEBUG
        return *_Mybuf._Get();
    }

    template <class _Elem>
    typename string<_Elem>::const_reference string<_Elem>::front() const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Mybuf._Size > 0, "attempt to access non-existent element");
#endif // _DEBUG
        return *_Mybuf._Get();
    }

    template <class _Elem>
    typename string<_Elem>::reference string<_Elem>::back() noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Mybuf._Size > 0, "attempt to access non-existent element");
#endif // _DEBUG
        return _Mybuf._Get()[_Mybuf._Size - 1];
    }

    template <class _Elem>
    typename string<_Elem>::const_reference string<_Elem>::back() const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Mybuf._Size > 0, "attempt to access non-existent element");
#endif // _DEBUG
        return _Mybuf._Get()[_Mybuf._Size - 1];
    }

    template <class _Elem>
    typename string<_Elem>::pointer string<_Elem>::data() noexcept {
        return _Mybuf._Get();
    }

    template <class _Elem>
    typename string<_Elem>::const_pointer string<_Elem>::data() const noexcept {
        return _Mybuf._Get();
    }

    template <class _Elem>
    typename string<_Elem>::const_pointer string<_Elem>::c_str() const noexcept {
        return _Mybuf._Get();
    }

    template <class _Elem>
    bool string<_Elem>::empty() const noexcept {
        return _Mybuf._Size == 0;
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::capacity() const noexcept {
        return _Mybuf._Capacity;
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::size() const noexcept {
        return _Mybuf._Size;
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::max_size() noexcept {
        return (::std::min)(static_cast<size_type>(PTRDIFF_MAX),
            static_cast<size_type>(-1) / sizeof(value_type)) - 1;
    }

    template <class _Elem>
    string_view<_Elem> string<_Elem>::view() const noexcept {
        return string_view<_Elem>{_Mybuf._Get(), _Mybuf._Size};
    }

    template <class _Elem>
    void string<_Elem>::reserve(size_type _New_capacity) {
        if (_Mybuf._Capacity >= _New_capacity || _New_capacity <= _Small_buffer_capacity) {
            // already reserved requested or more capacity, or fits in small buffer, do nothing
            return;
        }

        pointer _New_ptr = _Allocate_space_for_capacity(_New_capacity); // may throw
        traits_type::copy(_New_ptr, _Mybuf._Get(), _Mybuf._Size + 1); // copy data and null-terminator
        _Mybuf._Deallocate_if_large();
        _Mybuf._Capacity = _New_capacity;
        _Mybuf._Large    = _New_ptr;
    }

    template <class _Elem>
    typename string<_Elem>::size_type
        string<_Elem>::copy(pointer _Dest, size_type _Count, const size_type _Off) const {
        return view().copy(_Dest, _Count, _Off);
    }

    template <class _Elem>
    void string<_Elem>::swap(string& _Other) noexcept {
        if (this != ::std::addressof(_Other)) {
            const bool _This_small  = _Mybuf._Is_small();
            const bool _Other_small = _Other._Mybuf._Is_small();
            if (_This_small && _Other_small) { // swap two small buffers
                // copy null-terminators to skip assigning them after copying
                value_type _Temp[_Small_buffer_size];
                traits_type::copy(_Temp, _Mybuf._Small, _Mybuf._Size + 1);
                traits_type::copy(_Mybuf._Small, _Other._Mybuf._Small, _Other._Mybuf._Size + 1);
                traits_type::copy(_Other._Mybuf._Small, _Temp, _Mybuf._Size + 1);
            } else if (_This_small) { // swap small with large
                _Mybuf._Swap_small_with_large(_Other._Mybuf);
            } else if (_Other_small) { // swap large with small
                _Other._Mybuf._Swap_small_with_large(_Mybuf);
            } else { // swap two large buffers
                ::std::swap(_Mybuf._Large, _Other._Mybuf._Large);
            }

            ::std::swap(_Mybuf._Size, _Other._Mybuf._Size);
            ::std::swap(_Mybuf._Capacity, _Other._Mybuf._Capacity);
        }
    }

    template <class _Elem>
    void string<_Elem>::clear() noexcept {
        if (_Mybuf._Size > 0) {
            *_Mybuf._Get() = static_cast<value_type>(0);
            _Mybuf._Size   = 0;
        }
    }

    template <class _Elem>
    void string<_Elem>::resize(const size_type _New_size, const value_type _Ch) {
        if (_New_size <= _Mybuf._Size) { // decrease buffer size
            _Mybuf._Size                = _New_size;
            _Mybuf._Get()[_Mybuf._Size] = static_cast<value_type>(0);
        } else { // increase buffer size
            append(_New_size - _Mybuf._Size, _Ch);
        }
    }

    template <class _Elem>
    void string<_Elem>::expand(const size_type _Count, const value_type _Ch) {
        if (_Count == 0) { // no expanding, do nothing
            return;
        }

        if (_Mybuf._Size + _Count <= _Mybuf._Capacity) { // found enough space, don't reallocate memory
            pointer _Old_ptr = _Mybuf._Get();
            traits_type::assign(_Old_ptr + _Mybuf._Size, _Count, _Ch);
            _Mybuf._Size          += _Count;
            _Old_ptr[_Mybuf._Size] = static_cast<value_type>(0);
            return;
        }

        _Reallocate_insert_back(_Count, _Ch);
    }

    template <class _Elem>
    void string<_Elem>::shrink(const size_type _Count) noexcept {
        if (_Count == 0) { // no shrinking, do nothing
            return;
        }

        _Mybuf._Size               -= (::std::min)(_Count, _Mybuf._Size);
        _Mybuf._Get()[_Mybuf._Size] = static_cast<value_type>(0);
    }

    template <class _Elem>
    void string<_Elem>::shrink_to_fit() {
        if (_Mybuf._Is_small()) { // small string is always considered as fit
            return;
        }

        if (_Mybuf._Size <= _Small_buffer_capacity) { // switch to small buffer
            _Mybuf._Switch_to_small();
            return;
        }

        size_type _New_capacity = _Mybuf._Size | _Alloc_mask;
        if (_New_capacity < _Mybuf._Capacity) { // worth shrinking, do it
            pointer _New_ptr = _Allocate_space_for_capacity(_New_capacity); // may throw
            traits_type::copy(_New_ptr, _Mybuf._Large, _Mybuf._Size + 1); // copy null-terminator too
            _Mybuf._Deallocate_if_large();
            _Mybuf._Capacity = _New_capacity;
            _Mybuf._Large    = _New_ptr;
        }
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::assign(const size_type _Count, const value_type _Ch) {
        if (_Count <= _Mybuf._Capacity) { // found enough space, don't reallocate memory
            pointer _Old_ptr = _Mybuf._Get();
            traits_type::assign(_Old_ptr, _Count, _Ch);
            _Mybuf._Size           = _Count;
            _Old_ptr[_Mybuf._Size] = static_cast<value_type>(0);
            return *this;
        }

        _Reallocate_assign(_Count, _Ch);
        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::assign(const string& _Str) {
        if (this == ::std::addressof(_Str)) { // must not be this string
            return *this;
        }

        return assign(_Str._Mybuf._Get(), _Str._Mybuf._Size);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::assign(string&& _Str) noexcept {
        if (this == ::std::addressof(_Str)) { // must not be this string
            return *this;
        }

        _Take_contents(_Str);
        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::assign(const_pointer _Ptr, const size_type _Count) {
        if (_Count <= _Mybuf._Capacity) { // found enough space, don't reallocate memory
            pointer _Old_ptr = _Mybuf._Get();
            traits_type::copy(_Old_ptr, _Ptr, _Count);
            _Mybuf._Size           = _Count;
            _Old_ptr[_Mybuf._Size] = static_cast<value_type>(0);
            return *this;
        }

        _Reallocate_assign(_Ptr, _Count);
        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::assign(const_pointer _Ptr) {
        return assign(_Ptr, traits_type::length(_Ptr));
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::assign(const string_view<_Elem> _Str) {
        return assign(_Str.data(), _Str.size());
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::append(const size_type _Count, const value_type _Ch) {
        if (_Mybuf._Size + _Count <= _Mybuf._Capacity) { // found enough space, don't reallocate memory
            pointer _Old_ptr = _Mybuf._Get();
            traits_type::assign(_Old_ptr + _Mybuf._Size, _Count, _Ch);
            _Mybuf._Size          += _Count;
            _Old_ptr[_Mybuf._Size] = static_cast<value_type>(0);
            return *this;
        }

        _Reallocate_insert_back(_Count, _Ch);
        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::append(const string& _Str) {
        return append(_Str._Mybuf._Get(), _Str._Mybuf._Size);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::append(const_pointer _Ptr, const size_type _Count) {
        if (_Mybuf._Size + _Count <= _Mybuf._Capacity) { // found enough space, don't reallocate memory
            pointer _Old_ptr = _Mybuf._Get();
            traits_type::copy(_Old_ptr + _Mybuf._Size, _Ptr, _Count);
            _Mybuf._Size          += _Count;
            _Old_ptr[_Mybuf._Size] = static_cast<value_type>(0);
            return *this;
        }

        _Reallocate_insert_back(_Ptr, _Count);
        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::append(const_pointer _Ptr) {
        return append(_Ptr, traits_type::length(_Ptr));
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::append(const string_view<_Elem> _Str) {
        return append(_Str.data(), _Str.size());
    }

    template <class _Elem>
    void string<_Elem>::push_back(const value_type _Ch) {
        if (_Mybuf._Size + 1 <= _Mybuf._Capacity) { // found enough space, don't reallocate memory
            pointer _Old_ptr         = _Mybuf._Get();
            _Old_ptr[_Mybuf._Size]   = _Ch;
            _Old_ptr[++_Mybuf._Size] = static_cast<value_type>(0);
            return;
        }

        _Reallocate_insert_back(1, _Ch);
    }

    template <class _Elem>
    void string<_Elem>::pop_back() noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Mybuf._Size > 0, "pop_back() called on empty string")
#endif // _DEBUG
        --_Mybuf._Size;
        _Mybuf._Get()[_Mybuf._Size] = static_cast<value_type>(0);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::erase(const size_type _Off, size_type _Count) {
        _Check_offset(_Off);
        _Count = (::std::min)(_Count, _Mybuf._Size - _Off);
        if (_Count > 0) { // remove some characters
            pointer _Old_ptr          = _Mybuf._Get() + _Off;
            const size_type _New_size = _Mybuf._Size - _Count;
            traits_type::move(_Old_ptr, _Old_ptr + _Count, _New_size - _Off + 1); // move null-terminator too
            _Mybuf._Size = _New_size;
        }

        return *this;
    }

    template <class _Elem>
    typename string<_Elem>::iterator string<_Elem>::erase(const const_iterator _Where) {
        const size_type _Off = static_cast<size_type>(_Where._Myptr - _Mybuf._Get());
        erase(_Off, 1);
        return begin() + static_cast<difference_type>(_Off);
    }

    template <class _Elem>
    typename string<_Elem>::iterator string<_Elem>::erase(const const_iterator _First, const const_iterator _Last) {
        const size_type _Off = static_cast<size_type>(_First._Myptr - _Mybuf._Get());
        erase(_Off, static_cast<size_type>(_Last._Myptr - _First._Myptr));
        return begin() + static_cast<difference_type>(_Off);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::insert(const size_type _Off, const size_type _Count, const value_type _Ch) {
        _Check_offset_for_insertion(_Off);
        if (_Mybuf._Size + _Count <= _Mybuf._Capacity) { // found enough space, don't reallocate memory
            pointer _Old_ptr = _Mybuf._Get() + _Off;
            traits_type::move(_Old_ptr + _Count, _Old_ptr, _Mybuf._Size - _Off + 1); // move null-terminator too
            traits_type::assign(_Old_ptr, _Count, _Ch);
            _Mybuf._Size += _Count;
            return *this;
        }

        _Reallocate_insert_at(_Off, _Count, _Ch);
        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::insert(const size_type _Off, const_pointer _Ptr) {
        return insert(_Off, _Ptr, traits_type::length(_Ptr));
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::insert(const size_type _Off, const_pointer _Ptr, const size_type _Count) {
        _Check_offset_for_insertion(_Off);
        if (_Mybuf._Size + _Count <= _Mybuf._Capacity) { // found enough space, don't reallocate memory
            pointer _Old_ptr = _Mybuf._Get() + _Off;
            traits_type::move(_Old_ptr + _Count, _Old_ptr, _Mybuf._Size - _Off + 1); // move null-terminator too
            traits_type::copy(_Old_ptr, _Ptr, _Count);
            _Mybuf._Size += _Count;
            return *this;
        }

        _Reallocate_insert_at(_Off, _Ptr, _Count);
        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::insert(const size_type _Off, const string& _Str) {
        return insert(_Off, _Str._Mybuf._Get(), _Str._Mybuf._Size);
    }

    template <class _Elem>
    typename string<_Elem>::iterator string<_Elem>::insert(const const_iterator _Where, const value_type _Ch) {
        const size_type _Off = static_cast<size_type>(_Where._Myptr - _Mybuf._Get());
        insert(_Off, 1, _Ch);
        return begin() + static_cast<difference_type>(_Off);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::insert(const size_type _Off, const string_view<_Elem> _Str) {
        return insert(_Off, _Str.data(), _Str.size());
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::replace(const size_type _Off, size_type _Count, const string& _Str) {
        return replace(_Off, _Count, _Str._Mybuf._Get(), _Str._Mybuf._Size);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::replace(
        const const_iterator _First, const const_iterator _Last, const string& _Str) {
        return replace(static_cast<size_type>(_First._Myptr - _Mybuf._Get()),
            static_cast<size_type>(_Last._Myptr - _First._Myptr), _Str._Mybuf._Get(), _Str._Mybuf._Size);
    }
    
    template <class _Elem>
    string<_Elem>& string<_Elem>::replace(
        const size_type _Off, size_type _Count, const_pointer _Ptr, const size_type _Ptr_count) {
        _Check_offset(_Off);
        _Count = (::std::min)(_Count, _Mybuf._Size - _Off);
        if (_Count >= _Ptr_count) { // size will either remain the same or become smaller
            const size_type _Reduction = _Count - _Ptr_count;
            pointer _Old_ptr           = _Mybuf._Get() + _Off;
            traits_type::copy(_Old_ptr, _Ptr, _Ptr_count);
            if (_Reduction > 0) { // become smaller
                traits_type::move(_Old_ptr + _Ptr_count, _Old_ptr + _Count, _Mybuf._Size - _Off - _Count + 1);
                _Mybuf._Size -= _Reduction;
            }

            return *this;
        }

        const size_type _Growth = _Ptr_count - _Count;
        if (_Mybuf._Size + _Growth <= _Mybuf._Capacity) { // found enough space, don't reallocate memory
            pointer _Old_ptr = _Mybuf._Get() + _Off;
            traits_type::move(_Old_ptr + _Ptr_count, _Old_ptr + _Count, _Mybuf._Size - _Off - _Count + 1);
            traits_type::copy(_Old_ptr, _Ptr, _Ptr_count);
            _Mybuf._Size += _Growth;
            return *this;
        }

        _Reallocate_replace(_Off, _Count, _Ptr, _Ptr_count);
        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::replace(
        const const_iterator _First, const const_iterator _Last, const_pointer _Ptr, const size_type _Count) {
        return replace(static_cast<size_type>(_First._Myptr - _Mybuf._Get()),
            static_cast<size_type>(_Last._Myptr - _First._Myptr), _Ptr, _Count);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::replace(
        const const_iterator _First, const const_iterator _Last, const_pointer _Ptr) {
        return replace(static_cast<size_type>(_First._Myptr - _Mybuf._Get()),
            static_cast<size_type>(_Last._Myptr - _First._Myptr), _Ptr, traits_type::length(_Ptr));
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::replace(
        const size_type _Off, size_type _Count, const size_type _Ch_count, const value_type _Ch) {
        _Check_offset(_Off);
        _Count = (::std::min)(_Count, _Mybuf._Size);
        if (_Count >= _Ch_count) { // size will either remain the same or become smaller
            const size_type _Reduction = _Count - _Ch_count;
            pointer _Old_ptr           = _Mybuf._Get() + _Off;
            traits_type::assign(_Old_ptr, _Ch_count, _Ch);
            if (_Reduction > 0) { // become smaller
                traits_type::move(_Old_ptr + _Ch_count, _Old_ptr + _Count, _Mybuf._Size - _Off - _Count + 1);
                _Mybuf._Size -= _Reduction;
            }
        
            return *this;
        }

        const size_type _Growth = _Ch_count - _Count;
        if (_Mybuf._Size + _Growth <= _Mybuf._Capacity) { // found enough space, don't reallocate memory
            pointer _Old_ptr = _Mybuf._Get() + _Off;
            traits_type::move(_Old_ptr + _Ch_count, _Old_ptr + _Count, _Mybuf._Size - _Off - _Count + 1);
            traits_type::assign(_Old_ptr, _Ch_count, _Ch);
            _Mybuf._Size += _Growth;
            return *this;
        }

        _Reallocate_replace(_Off, _Count, _Ch_count, _Ch);
        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::replace(const size_type _Off, size_type _Count, const_pointer _Ptr) {
        return replace(_Off, _Count, _Ptr, traits_type::length(_Ptr));
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::replace(
        const const_iterator _First, const const_iterator _Last, const size_type _Count, const value_type _Ch) {
        return replace(static_cast<size_type>(_First._Myptr - _Mybuf._Get()),
            static_cast<size_type>(_Last._Myptr - _First._Myptr), _Count, _Ch);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::replace(const size_type _Off, size_type _Count, const string_view<_Elem> _Str) {
        return replace(_Off, _Count, _Str.data(), _Str.size());
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::replace(
        const const_iterator _First, const const_iterator _Last, const string_view<_Elem> _Str) {
        return replace(static_cast<size_type>(_First._Myptr - _Mybuf._Get()),
            static_cast<size_type>(_Last._Myptr - _First._Myptr), _Str.data(), _Str.size());
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::find(const string& _Str, const size_type _Off) const {
        return view().find(_Str.view(), _Off);
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::find(
        const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept {
        return view().find(_Ptr, _Off, _Count);
    }

    template <class _Elem>
    typename string<_Elem>::size_type
        string<_Elem>::find(const value_type _Ch, const size_type _Off) const noexcept {
        return view().find(_Ch, _Off);
    }

    template <class _Elem>
    typename string<_Elem>::size_type
        string<_Elem>::find(const string_view<_Elem> _Str, const size_type _Off) const noexcept {
        return view().find(_Str, _Off);
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::rfind(const string& _Str, const size_type _Off) const {
        return view().rfind(_Str.view(), _Off);
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::rfind(
        const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept {
        return view().rfind(_Ptr, _Off, _Count);
    }

    template <class _Elem>
    typename string<_Elem>::size_type
        string<_Elem>::rfind(const_pointer _Ptr, const size_type _Off) const noexcept {
        return view().rfind(_Ptr, _Off);
    }

    template <class _Elem>
    typename string<_Elem>::size_type
        string<_Elem>::rfind(const value_type _Ch, const size_type _Off) const noexcept {
        return view().rfind(_Ch, _Off);
    }

    template <class _Elem>
    typename string<_Elem>::size_type
        string<_Elem>::rfind(const string_view<_Elem> _Str, const size_type _Off) const noexcept {
        return view().rfind(_Str, _Off);
    }

    template <class _Elem>
    int string<_Elem>::compare(const string& _Str) const {
        return view().compare(_Str.view());
    }

    template <class _Elem>
    int string<_Elem>::compare(const_pointer _Ptr, const size_type _Count) const noexcept {
        return view().compare(_Ptr, _Count);
    }

    template <class _Elem>
    int string<_Elem>::compare(const_pointer _Ptr) const noexcept {
        return view().compare(_Ptr);
    }

    template <class _Elem>
    int string<_Elem>::compare(const string_view<_Elem> _Str) const noexcept {
        return view().compare(_Str);
    }

    template <class _Elem>
    bool string<_Elem>::starts_with(const string_view<_Elem> _Str) const noexcept {
        return view().starts_with(_Str);
    }

    template <class _Elem>
    bool string<_Elem>::starts_with(const value_type _Ch) const noexcept {
        return view().starts_with(_Ch);
    }

    template <class _Elem>
    bool string<_Elem>::starts_with(const_pointer _Ptr) const noexcept {
        return view().starts_with(_Ptr);
    }

    template <class _Elem>
    bool string<_Elem>::ends_with(const string_view<_Elem> _Str) const noexcept {
        return view().ends_with(_Str);
    }

    template <class _Elem>
    bool string<_Elem>::ends_with(const value_type _Ch) const noexcept {
        return view().ends_with(_Ch);
    }

    template <class _Elem>
    bool string<_Elem>::ends_with(const_pointer _Ptr) const noexcept {
        return view().ends_with(_Ptr);
    }

    template <class _Elem>
    bool string<_Elem>::contains(const string_view<_Elem> _Str) const noexcept {
        return find(_Str) != npos;
    }

    template <class _Elem>
    bool string<_Elem>::contains(const value_type _Ch) const noexcept {
        return find(_Ch) != npos;
    }

    template <class _Elem>
    bool string<_Elem>::contains(const_pointer _Ptr) const noexcept {
        return find(string_view<_Elem>{_Ptr, traits_type::length(_Ptr)}) != npos;
    }

    template <class _Elem>
    string<_Elem> string<_Elem>::substr(const size_type _Off, size_type _Count) const {
        _Check_offset(_Off);
        _Count = (::std::min)(_Count, _Mybuf._Size - _Off); // trim number of characters
        return string{_Mybuf._Get() + _Off, _Count};
    }

    template class _MJSTR_API string<byte_t>;
    template class _MJSTR_API string<char>;
    template class _MJSTR_API string<wchar_t>;
} // namespace mjx