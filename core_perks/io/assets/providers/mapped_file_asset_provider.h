// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/assets/providers/asset_provider.h"
#include "core_perks/io/file.h"

namespace cp
{
	class MappedFileAssetProvider : public AssetProvider
	{
	public:
		MappedFileAssetProvider(FileHandle&& file_handle);
		~MappedFileAssetProvider() override;

	protected:
		MappedAssetData map_asset(AssetEntry& entry) override;
		void unmap_asset(MappedAssetData& data) override;
		
	private:
		FileHandle file_handle_;
	};
}
