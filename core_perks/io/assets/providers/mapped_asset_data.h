// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/assets/providers/mapped_region.h"

namespace cp
{
	class AssetEntry; class AssetProvider;

	class MappedAssetData
	{
	public:
		MappedAssetData() = default;
		MappedAssetData(AssetProvider& provider, AssetEntry& entry, MappedRegion&& region);

		void* data() const { return region_.data(); }
		uint64 size() const { return region_.size(); }
		AssetEntry* get_entry() const { return entry_; }
		AssetProvider* get_provider() const { return provider_; }

	private:
		AssetProvider* provider_ = nullptr;
		AssetEntry* entry_= nullptr;
		MappedRegion region_;
	};
}
