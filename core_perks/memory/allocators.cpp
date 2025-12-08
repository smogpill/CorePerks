// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/memory/allocators.h"

namespace cp
{
	void* aligned_alloc(size_t alignment, size_t size, AllocTag tag)
	{
#ifdef CP_MSVC
		// Visual Studio does not support C++17's std::aligned_alloc(): https://stackoverflow.com/questions/62962839/stdaligned-alloc-missing-from-visual-studio-2019c
		return _aligned_malloc(size, alignment);
#else
		return std::aligned_alloc(alignment, size);
#endif
	}

	void free(void* ptr)
	{
#ifdef CP_MSVC
		_aligned_free(ptr);
#else
		std::free(ptr);
#endif
	}
}
