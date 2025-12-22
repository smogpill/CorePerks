// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "mapped_file_asset_provider.h"
#include "core_perks/io/files/file_handle.h"

namespace cp
{
	MappedFileAssetProvider::MappedFileAssetProvider(FileHandle&& file_handle)
		: file_handle_(std::move(file_handle))
	{
	}

	MappedFileAssetProvider::~MappedFileAssetProvider()
	{
	}

	MappedAssetData MappedFileAssetProvider::map_asset(AssetEntry& entry)
	{
		return MappedAssetData(*this, entry, file_handle_.map());
	}

	void MappedFileAssetProvider::unmap_asset(MappedAssetData& data)
	{
	}
}
