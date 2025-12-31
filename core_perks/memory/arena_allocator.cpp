// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/memory/arena_allocator.h"
#include "core_perks/memory/memory.h"

namespace cp
{
	void* ArenaAllocator::allocate(uint64 size, uint64 alignment)
	{
		if (size == 0)
			return nullptr;

		offset_ = align_up(offset_, alignment);
		const uint64 start = offset_;
		offset_ += size;
		if (offset_ > capacity_)
			CP_FATAL("Arena allocator is full");
		return memory_ + start;
	}
}
	