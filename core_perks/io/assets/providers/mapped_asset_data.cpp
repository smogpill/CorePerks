// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/assets/providers/mapped_asset_data.h"

namespace cp
{
	MappedAssetData::MappedAssetData(AssetProvider& provider, AssetEntry& entry, MappedRegion&& region)
		: provider_(&provider), entry_(&entry), region_(std::move(region))
	{
	}
}
