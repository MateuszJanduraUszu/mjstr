// heap_allocator.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJMEM_HEAP_ALLOCATOR_HPP_
#define _MJMEM_HEAP_ALLOCATOR_HPP_
#include <mjmem/allocator.hpp>
#include <mjmem/api.hpp>

namespace mjx {
    class _MJMEM_API heap_allocator : public allocator { // stateless memory allocator
    public:
        using value_type      = allocator::value_type;
        using size_type       = allocator::size_type;
        using difference_type = allocator::difference_type;
        using pointer         = allocator::pointer;

        heap_allocator() noexcept;
        heap_allocator(const heap_allocator& _Other) noexcept;
        heap_allocator(heap_allocator&& _Other) noexcept;
        ~heap_allocator() noexcept override;

        heap_allocator& operator=(const heap_allocator& _Other) noexcept;
        heap_allocator& operator=(heap_allocator&& _Other) noexcept;

        // allocates uninitialized storage
        pointer allocate(const size_type _Count) override;

        // allocates uninitialized storage with the specifed alignment
        pointer allocate_aligned(const size_type _Count, const size_type _Align) override;

        // deallocates storage
        void deallocate(pointer _Ptr, const size_type _Count) noexcept override;

        // returns the largest supported allocation size
        size_type max_size() const noexcept override;

        // compares for equality with another allocator
        bool is_equal(const allocator& _Al) const noexcept override;
    };
} // namespace mjx

#endif // _MJMEM_HEAP_ALLOCATOR_HPP_