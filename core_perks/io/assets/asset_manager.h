// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/singleton.h"
#include "core_perks/io/assets/asset.h"
#include "core_perks/io/assets/asset_entry.h"
#include "core_perks/io/assets/asset_handle.h"
#include "core_perks/io/assets/providers/mapped_asset_data.h"

namespace cp
{
	class AssetManager : public Singleton<AssetManager>
	{
	public:
		~AssetManager();

		// Assets path
		void set_assets_path(const std::string& path);
		const std::string& get_assets_path() const { return assets_path_; }

		// Cache path
		void set_cache_path(const std::string& path);
		const std::string& get_cache_path() const { return cache_path_; }

		void register_provider(AssetProvider& provider);
		void unregister_provider(AssetProvider& provider);

	private:
		friend class AssetEntry;
		friend class Asset;
		friend class AssetHandle;

		AssetEntry* get_or_create_entry(const std::string& id, const Type& type);
		void destroy_entry(AssetEntry& entry);
		void add_request(const AssetHandle& request);
		void process_requests();
		void on_entry_updated(AssetEntry& entry);
		MappedAssetData map_asset(AssetEntry& entry);

		mutable std::mutex mutex_;
		std::unordered_map<uint64, AssetEntry*> map_;
		std::queue<AssetHandle> requests_;
		std::string assets_path_;
		std::string cache_path_;
		std::vector<Asset*> providers_;
		uint max_parallel_updates_ = 4;

		/*
		Streaming
		std::queue<Request> immediate_queue_; // Must load now (player weapon, current mission)
		std::queue<Request> high_priority_queue_; // Visible/nearby
		std::queue<Request> low_priority_queue_; // Predictive/prefetch
		std::queue<Request> unload_queue_;
		*/
	};
}
