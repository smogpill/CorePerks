// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	template <class T>
	constexpr T align_up(const T val, const T alignment) { return (val + (alignment - 1)) & ~(alignment - 1); }
	template <typename T>
	constexpr T align_down(const T val, const T alignment) { /*CP_ASSERT(is_power_of_two(alignment)); */ return val & ~(alignment - 1); }
	constexpr bool is_aligned(const void* ptr, size_t alignment) { return (std::bit_cast<uintptr>(ptr) & (alignment - 1)) == 0; }
	void fill_as_deleted(void* ptr, size_t size);

	template <typename T>
	CP_FORCE_INLINE void swap_memory(T& _a, T& _b)
	{
		std::byte tmp[sizeof(T)];
		std::memcpy(tmp, &_a, sizeof(_a));
		std::memcpy(&_a, &_b, sizeof(_b));
		std::memcpy(&_b, tmp, sizeof(tmp));
	}

	struct AllocTag
	{
		uint8 _tag = 0;
	};

	/*
	template <class T>
	class TaggedAllocator
	{
	public:
		AllocTag _tag;
	};
	*/

	void* aligned_alloc(size_t alignment, size_t size, AllocTag tag);
	void free(void* ptr);

	class StackAllocator
	{
	public:

	};

	class ArenaAllocator
	{
	public:

	};

}
