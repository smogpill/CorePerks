// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/assets/asset.h"
#include "core_perks/io/assets/asset_manager.h"

namespace cp
{
	CP_DEFINE_CLASS(Asset)
	{

	}

	void Asset::map_sub_asset(AssetEntry& entry, MappedAssetData& mapped_data)
	{
		mapped_data = MappedAssetData();
	}

	void Asset::unmap_sub_asset(MappedAssetData& mapped_data)
	{
		mapped_data = MappedAssetData();
	}
}
