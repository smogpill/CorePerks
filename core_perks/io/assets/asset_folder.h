// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/assets/asset.h"
#include "core_perks/io/file.h"

namespace cp
{
	class AssetEntry;
	class AssetHandle;

	class AssetFolder : public Asset
	{
		CP_BASE(Asset);
		CP_CLASS(AssetFolder);
	public:
		AssetFolder(const std::string& path);

	protected:
		bool on_load() override;
		MappedAssetData map_sub_asset(const AssetHandle& asset) override;
		void unmap_sub_asset(MappedAssetData& mapped_data) override;

	private:
		struct AssetInfo
		{
			FileHandle file_;
			uint64 asset_id_hash_ = 0;
		};

		AssetInfo& get_or_open_file(const AssetEntry& entry);
		AssetInfo* get_file(const AssetEntry& entry) const;

		std::filesystem::path path_;
		std::vector<AssetInfo*> open_files_;
	};
}
