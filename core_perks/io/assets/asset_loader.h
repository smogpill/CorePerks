// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/assets/asset_entry.h"
#include "core_perks/io/assets/asset_manager.h"

namespace cp
{
	template <class T> class TypedAssetHandle;

	class AssetLoader
	{
	public:
		struct DependencyOptions
		{
			bool _optional = false;
		};

		AssetLoader(AssetEntry& entry);
		template <class T>
		TypedAssetHandle<T> add_dependency(const std::string& id, const DependencyOptions& options = DependencyOptions());
		const std::string& get_id() const;
		std::string get_asset_path() const;
		bool read_as_binary_file(std::vector<uint8>& content);
		bool path_exists() const;

	private:
		AssetEntry& _entry;
	};

	template <class T>
	TypedAssetHandle<T> AssetLoader::add_dependency(const std::string& id, const DependencyOptions& options)
	{
		TypedAssetHandle<T> handle(id);
		if (handle.entry_->path_exists() || !options._optional)
		{
			//handle.entry_->_loading_parent = &_entry;
			_entry.add_loading_dependency();
			handle.load_async();
		}
		return handle;
	}
}
