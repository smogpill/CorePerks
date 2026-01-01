// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/providers/asset_provider.h"
#include "core_perks/io/file.h"

#if 0
namespace cp
{
	class FolderAssetProvider : public AssetProvider
	{
	public:
		FolderAssetProvider(const std::string& path);

		MappedResourceData map_resource(ResourceEntry& entry) override;

	protected:
		void unmap_asset(MappedResourceData& data) override;

	private:
		struct ResourceInfo
		{
			FileHandle file_;
			uint64 resource_id_hash_ = 0;
		};

		ResourceInfo& get_or_open_file(const ResourceEntry& entry);
		ResourceInfo* get_file(const ResourceEntry& entry) const;

		std::filesystem::path folder_path_;
		std::vector<ResourceInfo*> open_files_;
	};
}
#endif