// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/memory/memory.h"

namespace cp
{
	void fill_as_deleted(void* ptr_, size_t size)
	{
		uint8* ptr = static_cast<uint8*>(ptr_);
		while (size >= 8)
		{
			*reinterpret_cast<uint64*>(ptr) = 0xfefefefefefefefe;
			ptr += 8;
			size -= 8;
		}
		while (size)
		{
			*ptr = 0xfe;
			++ptr;
			--size;
		}
	}
}
