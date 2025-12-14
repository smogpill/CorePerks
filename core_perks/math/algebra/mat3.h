// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/math/algebra/vec3.h"

namespace cp
{
	class alignas(16) Mat3f
	{
	public:
	private:
		Vec3f x_;
		Vec3f y_;
		Vec3f z_;
	};
}
