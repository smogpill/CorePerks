// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	class BinaryOutputStream;
	class AssetLoader;
	class MappedAssetData;

	class Asset
	{
		CP_CLASS(Asset);
	public:
		virtual ~Asset() = default;

		virtual bool on_load(AssetEntry& entry) = 0 { return true; }
		virtual bool on_dependency_loaded(AssetEntry& dependency) { return true; }
		virtual bool on_all_dependencies_loaded() { return true; }
		virtual void on_store(cp::BinaryOutputStream& stream) const {}

	protected:
		virtual void map_sub_asset(AssetEntry& entry, MappedAssetData& mapped_data);
		virtual void unmap_sub_asset(MappedAssetData& mapped_data);
	private:
		uint32 _version = 0;
		std::vector<UntypedAssetHandle> dependencies_;
		std::vector<UntypedAssetHandle> dependents_;
	};
}
