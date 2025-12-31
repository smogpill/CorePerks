// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/memory/memory.h"

namespace cp
{
	template <class T>
	struct PaddedAtomic
	{
		alignas(cache_line_size) std::atomic<T> atomic_;
		uint8 padding_[cache_line_size - sizeof(decltype(atomic_))];
	};
}
