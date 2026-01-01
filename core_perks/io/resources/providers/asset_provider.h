// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/providers/mapped_resource_data.h"

#if 0
namespace cp
{
	class ResourceEntry;

	class AssetProvider
	{
	public:
		virtual ~AssetProvider() = default;

		MappedResourceData map_resource(ResourceEntry& entry) = 0;

	protected:
		friend class MappedResourceData;
		void unmap_asset(MappedResourceData& data) = 0;
	};
}
#endif
