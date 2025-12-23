// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/assets/asset.h"
#include "core_perks/io/assets/asset_handle.h"
#include "core_perks/io/assets/providers/asset_provider.h"

namespace cp
{
	class AssetPack : public Asset
	{
		CP_BASE(Asset);
		CP_CLASS(AssetPack);
	public:
		void add_sub_resource(const AssetHandle& handle);

	protected:
		virtual bool on_load(AssetEntry& entry) override;
		virtual MappedAssetData map_sub_asset(const AssetHandle& asset) override;
		virtual void unmap_sub_asset(MappedAssetData& data) override;

	private:
		struct SubAssetInfo
		{
			AssetHandle handle_;
			uint64 offset_ = 0;
			uint64 size_ = 0;
		};
		std::vector<SubAssetInfo> sub_assets_;
		//std::unorder_map<uint64, SubAssetInfo*> id_hash_to_sub_asset_;
		FileHandle file_handle_;
	};
}
