// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	inline constexpr uint8 cache_line_size = 64;
	inline constexpr uint16 page_size = 4096;
	inline constexpr uint32 mega_huge_page_size = 2 * 1024 * 1024;
	inline constexpr uint32 giga_huge_page_size = 1024 * 1024 * 1024;

	template <class T>
	constexpr T align_up(const T val, const T alignment) { return (val + (alignment - 1)) & ~(alignment - 1); }
	template <typename T>
	constexpr T align_down(const T val, const T alignment) { /*CP_ASSERT(is_power_of_two(alignment)); */ return val & ~(alignment - 1); }
	constexpr bool is_aligned(const void* ptr, uint64 alignment) { return (std::bit_cast<uintptr>(ptr) & (alignment - 1)) == 0; }
	void mark_as_deleted(void* ptr, uint64 size);

	template <typename T>
	CP_FORCE_INLINE void swap_memory(T& a, T& b)
	{
		std::byte tmp[sizeof(T)];
		std::memcpy(tmp, &a, sizeof(a));
		std::memcpy(&a, &b, sizeof(b));
		std::memcpy(&b, tmp, sizeof(tmp));
	}

	struct AllocTag
	{
		uint8 tag_ = 0;
	};

	void* malloc(size_t) = delete;
	void* aligned_alloc(size_t, size_t) = delete;

	void* allocate(uint64 size, uint64 alignment = alignof(std::max_align_t), AllocTag tag = AllocTag());
	template <class T>
	T* allocate(AllocTag tag = AllocTag()) { (T*)allocate(sizeof(T), alignof(T), tag); }
	void free(void* ptr);
}

void* operator new(std::size_t size)
{
	return cp::allocate(size);
}

void* operator new(std::size_t size, std::align_val_t alignment)
{
	return cp::allocate(size, alignment);
}

void* operator new[](std::size_t size)
{
	return cp::allocate(size);
}

void* operator new[](std::size_t size, std::align_val_t alignment)
{
	return cp::allocate(size, alignment);
}

void operator delete(void* ptr)
{
	cp::free(ptr);
}

void operator delete(void* ptr, std::align_val_t)
{
	cp::free(ptr);
}

void operator delete[](void* ptr)
{
	cp::free(ptr);
}

void operator delete[](void* ptr, std::align_val_t)
{
	cp::free(ptr);
}
