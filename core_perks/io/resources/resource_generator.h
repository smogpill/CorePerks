// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

#if 0

namespace cp
{
	class Resource;

	class ResourceGenerator
	{
	public:
		virtual ~ResourceGenerator() = default;
		virtual bool generate(Resource& resource) = 0;
	};
}

#endif