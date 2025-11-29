// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	template <class T>
	constexpr T align_up(const T val, const T alignment)
	{
		return (val + (alignment - 1)) & ~(alignment - 1);
	}
}
