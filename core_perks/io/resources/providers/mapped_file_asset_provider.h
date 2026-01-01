// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/providers/asset_provider.h"
#include "core_perks/io/file.h"

#if 0
namespace cp
{
	class MappedFileAssetProvider : public AssetProvider
	{
	public:
		MappedFileAssetProvider(FileHandle&& file_handle);
		~MappedFileAssetProvider() override;

	protected:
		MappedResourceData map_resource(ResourceEntry& entry) override;
		void unmap_asset(MappedResourceData& data) override;
		
	private:
		FileHandle file_handle_;
	};
}
#endif
