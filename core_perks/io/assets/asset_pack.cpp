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

	void operator>>(BinaryInputStream& stream, AssetPack::SubAssetInfo& entry)
	{
		stream >> entry.handle_;
		stream >> entry.offset_;
		stream >> entry.size_;
	}

	void AssetPack::add_sub_resource(const AssetHandle& handle)
	{
		if (!contains(resources_, handle))
			resources_.push_back(handle);
	}

	bool AssetPack::on_load(AssetEntry& entry)
	{
		CP_TRY(Base::on_load(data));

		BinaryInputStream stream = data.get_stream();
		stream >> sub_assets_;
		return true;
	}

	MappedAssetData AssetPack::map_sub_asset(const AssetHandle& asset)
	{
		return MappedAssetData();
	}

	void AssetPack::unmap_sub_asset(MappedAssetData& data)
	{
	}
}
