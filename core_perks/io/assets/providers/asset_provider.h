// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/assets/providers/mapped_asset_data.h"

namespace cp
{
	class AssetEntry;

	class AssetProvider
	{
	public:
		virtual ~AssetProvider() = default;

		MappedAssetData map_asset(AssetEntry& entry) = 0;

	protected:
		friend class MappedAssetData;
		void unmap_asset(MappedAssetData& data) = 0;
	};
}
