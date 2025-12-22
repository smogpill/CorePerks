// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/assets/providers/asset_provider.h"
#include "core_perks/io/file.h"

namespace cp
{
	class FolderAssetProvider : public AssetProvider
	{
	public:
		FolderAssetProvider(const std::string& path);

		MappedAssetData map_asset(AssetEntry& entry) override;

	protected:
		void unmap_asset(MappedAssetData& data) override;

	private:
		struct AssetInfo
		{
			FileHandle file_;
			uint64 asset_id_hash_ = 0;
		};

		AssetInfo& get_or_open_file(const AssetEntry& entry);
		AssetInfo* get_file(const AssetEntry& entry) const;

		std::filesystem::path folder_path_;
		std::vector<AssetInfo*> open_files_;
	};
}
