// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/assets/providers/folder_asset_provider.h"

namespace cp
{
	FolderAssetProvider::FolderAssetProvider(const std::string& path)
		: folder_path_(path)
	{
	}

	MappedAssetData FolderAssetProvider::map_asset(AssetEntry& entry)
	{
		AssetInfo& info = get_or_open_file(entry);
		if (!info.file_.is_open())
			return MappedAssetData();
		MappedRegion region = info.file_.map(FileHandle::Access::READ_ONLY);
		return MappedAssetData(*this, entry, std::move(region));
	}

	void FolderAssetProvider::unmap_asset(MappedAssetData& data)
	{
		//AssetInfo* info = get_file(data);
	}

	AssetInfo& FolderAssetProvider::get_or_open_file(const AssetEntry& entry)
	{
		AssetInfo* info = get_file(entry);
		if (info)
			return *info;
		AssetInfo* new_info = new AssetInfo();
		const std::filesystem::path asset_path = folder_path_ / entry.get_asset_path();
		new_info->file_.open(asset_path.string(), FileHandle::Mode::READ);
		new_info->asset_id_hash_ = entry.get_id_hash();
		open_files_.push_back(new_info);
		return *new_info;
	}

	AssetInfo* FolderAssetProvider::get_file(const AssetEntry& entry) const
	{
		for (AssetInfo* info : open_files_)
		{
			if (info->asset_id_hash_ == entry.get_id_hash())
				return info;
		}
		return nullptr;
	}
}
