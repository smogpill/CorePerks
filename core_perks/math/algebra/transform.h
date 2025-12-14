// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/math/algebra/quat.h"
#include "core_perks/math/algebra/vec4.h"

namespace cp
{
	class alignas(32) Transformd
	{
	public:
		Vec4d translation_and_scale_;
		Quatf rotation_;
	};

	class alignas(16) Transformf
	{
	public:
		Vec4f translation_and_scale_;
		Quatf rotation_;
	};
}
