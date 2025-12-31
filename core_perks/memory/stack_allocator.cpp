// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/memory/stack_allocator.h"
#include "core_perks/memory/memory.h"

namespace cp
{
	namespace stack_allocator
	{
		static constexpr uint32 marker = 0x0D15EA5E;

		struct Header
		{
#ifdef CP_DEBUG
			uint32 pre_marker_;
#endif
			uint32 size_;
			uint32 header_alloc_size_;
#ifdef CP_DEBUG
			uint32 post_marker_;
#endif
		};

		// Necessary for (Header*)ptr - 1;
		static_assert(sizeof(Header) % alignof(Header) == 0);

		CP_FORCE_INLINE Header* get_header(void* ptr)
		{
			return (Header*)ptr - 1;
		}
	}

	void* StackAllocator::allocate(const size_t size, const size_t alignment)
	{
		if (size == 0) [[unlikely]]
			return nullptr;

		const size_t ptr_offset = align_up(offset_ + sizeof(stack_allocator::Header), std::max((size_t)alignment, alignof(stack_allocator::Header)));
		const size_t header_alloc_size = ptr_offset - offset_;
		offset_ = ptr_offset + size;
		if (offset_ > capacity_) [[unlikely]]
			CP_FATAL("Stack allocator is full :D");

		void* ptr = memory_ + ptr_offset;
		stack_allocator::Header* header = stack_allocator::get_header(ptr);
		CP_ASSERT(size <= std::numeric_limits<decltype(header->size_)>::max());
		header->size_ = (uint32)size;
		CP_ASSERT(header_alloc_size <= std::numeric_limits<decltype(header->header_alloc_size_)>::max());
		header->header_alloc_size_ = (uint32)header_alloc_size;
#ifdef CP_DEBUG
		header->pre_marker_ = stack_allocator::marker;
		header->post_marker_ = stack_allocator::marker;
#endif
		return ptr;
	}

	void StackAllocator::free(void* ptr)
	{
        if (!ptr)
            return;

		stack_allocator::Header* header = stack_allocator::get_header(ptr);
#ifdef CP_DEBUG
        if (header->pre_marker_ != stack_allocator::marker || header->post_marker_ != stack_allocator::marker) [[unlikely]]
            CP_FATAL("Stack alloc corruption XD (marker mismatch)");
#endif
		CP_ASSERT((uint8*)ptr + header->size_ == memory_ + offset_);
        offset_ = (size_t)ptr - header->header_alloc_size_;
#ifdef CP_DEBUG
		mark_as_deleted(memory_ + offset_, header->header_alloc_size_ + header->size_);
#endif
	}
}
