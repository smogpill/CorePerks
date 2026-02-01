// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2026 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/extern/vulkan/vulkan_conventions.h"

#ifdef CP_VULKAN
namespace cp
{
	Mat4f to_vulkan_view(const Mat4f& mat)
	{
		/*
		Mat4f z_flip;
		z_flip[2][2] = -1.0f;
		return mat * z_flip;
		*/
		return mat;
	}

	Mat4f to_vulkan_projection(const Mat4f& mat)
	{
		Mat4f vulkan_projection = mat;
		vulkan_projection[1][1] *= -1.0f;
		return vulkan_projection;
	}
}
#endif
