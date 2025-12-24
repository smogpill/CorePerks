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
		stream >> entry.id_hash_;
		stream >> entry.offset_;
		stream >> entry.size_;
	}

	bool AssetPack::on_load()
	{
		CP_TRY(Base::on_load());
		AssetEntry& entry = get_entry();
		MappedAssetData data = entry.get_mapped_data();
		BinaryInputStream stream = data.get_stream();
		stream >> sub_assets_;
		AssetManager::get().register_provider(*this);
		return true;
	}

	void AssetPack::on_unload()
	{
		AssetManager::get().unregister_provider(*this);
		sub_assets_.clear();
		Base::on_unload();
	}

	MappedAssetData AssetPack::map_sub_asset(const AssetHandle& asset)
	{
		SubAssetInfo* info = get_sub_resource_info(asset);
		if (info == nullptr)
			return MappedAssetData(asset);
		MappedRegion region = file_handle_.map_region(info->offset_, info->size_, FileHandle::Access::READ_ONLY);
		return MappedAssetData(asset, get_handle(), std::move(region));
	}

	void AssetPack::unmap_sub_asset(const AssetHandle& asset)
	{
		// Do nothing
	}

	AssetPack::SubAssetInfo* AssetPack::get_sub_resource_info(const AssetHandle& asset) const
	{
		const uint64 id_hash = asset->get_id_hash();
		for (SubAssetInfo& info : sub_assets_)
		{
			if (info.id_hash_ == id_hash)
				return &info;
		}
		return nullptr;
	}
}
