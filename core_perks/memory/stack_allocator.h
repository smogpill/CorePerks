// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	class StackAllocator
	{
	public:
		void* alloc(size_t size, size_t alignment = alignof(std::max_align_t));
		void free(void* ptr);

	private:
		uint8* memory_ = nullptr;
		size_t offset_ = 0;
		size_t capacity_ = 0;
	};
}
