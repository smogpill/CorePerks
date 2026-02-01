// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2026 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/math/algebra/mat4.h"

#ifdef CP_VULKAN
namespace cp
{
	[[nodiscard]] Mat4f to_vulkan_view(const Mat4f& mat);
	[[nodiscard]] Mat4f to_vulkan_projection(const Mat4f& mat);
}
#endif
