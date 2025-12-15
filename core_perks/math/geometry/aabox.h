// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/math/algebra/vec3.h"

namespace cp
{
	template <class T>
	class alignas(alignof(Vec3<T>)) AABox
	{
	public:
		Vec3<T> min_;
		Vec3<T> max_;
	};

	using AABoxf = AABox<float>;
	using AABoxd = AABox<double>;
}
