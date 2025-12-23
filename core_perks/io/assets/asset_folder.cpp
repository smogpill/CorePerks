// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/assets/asset_folder.h"
#include "core_perks/io/file.h"

namespace cp
{
	CP_DEFINE_CLASS(AssetFolder)
	{
	}

	AssetFolder::AssetFolder(const std::string& path)
		: path_(path)
	{
	}

	bool AssetFolder::on_load()
	{
		return Base::on_load();
	}

	MappedAssetData AssetFolder::map_sub_asset(AssetEntry& entry)
	{
		AssetInfo& info = get_or_open_file(entry);
		if (!info.file_.is_open())
			return MappedAssetData();
		return MappedAssetData(*this, entry, std::move(info.file_.map()));
	}

	void AssetFolder::unmap_asset(MappedAssetData& data)
	{
		//AssetInfo* info = get_file(data);
	}

	AssetFolder::AssetInfo& AssetFolder::get_or_open_file(const AssetEntry& entry)
	{
		AssetInfo* info = get_file(entry);
		if (info)
			return *info;
		AssetInfo* new_info = new AssetInfo();
		const std::filesystem::path asset_path = path_ / entry.get_asset_path();
		new_info->file_.open(asset_path.string(), FileHandle::Mode::READ);
		new_info->asset_id_hash_ = entry.get_id_hash();
		open_files_.push_back(new_info);
		return *new_info;
	}

	AssetFolder::AssetInfo* AssetFolder::get_file(const AssetEntry& entry) const
	{
		for (AssetInfo* info : open_files_)
		{
			if (info->asset_id_hash_ == entry.get_id_hash())
				return info;
		}
		return nullptr;
	}
}
