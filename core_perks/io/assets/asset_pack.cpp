// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/assets/asset_pack.h"
#include "core_perks/containers/vector_extensions.h"

namespace cp
{
	CP_DEFINE_CLASS(AssetPack)
	{
	}

	void AssetPack::add_resource(const UntypedAssetHandle& handle)
	{
		if (!contains(resources_, handle))
			resources_.push_back(handle);
	}

	bool AssetPack::on_load(AssetEntry& entry)
	{
		return false;
	}

	MappedAssetData AssetPack::map_asset(AssetEntry& entry)
	{
		return MappedAssetData();
	}

	void AssetPack::unmap_asset(MappedAssetData& data)
	{
	}
}
