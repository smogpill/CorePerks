// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "mapped_file_asset_provider.h"

#if 0
namespace cp
{
	MappedFileAssetProvider::MappedFileAssetProvider(FileHandle&& file_handle)
		: file_handle_(std::move(file_handle))
	{
	}

	MappedFileAssetProvider::~MappedFileAssetProvider()
	{
	}

	MappedResourceData MappedFileAssetProvider::map_resource(ResourceEntry& entry)
	{
		return MappedResourceData(*this, entry, file_handle_.map());
	}

	void MappedFileAssetProvider::unmap_asset(MappedResourceData& data)
	{
	}
}
#endif
