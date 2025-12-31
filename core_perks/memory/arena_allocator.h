// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	class ArenaAllocator
	{
	public:
		void* allocate(uint64 size, uint64 alignment = alignof(std::max_align_t));
		void free(void*) {}
		void reset() { offset_ = 0; }

	private:
		uint8* memory_ = nullptr;
		uint64 offset_ = 0;
		uint64 capacity_ = 0;
	};
}
