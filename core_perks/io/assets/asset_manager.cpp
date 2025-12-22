// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/assets/asset_manager.h"
#include "core_perks/io/assets/asset_loader.h"
#include "core_perks/io/assets/asset_provider.h"
#include "core_perks/io/assets/asset_entry.h"
#include "core_perks/containers/vector_extensions.h"

namespace cp
{
	AssetManager::~AssetManager()
	{
		std::scoped_lock lock(mutex_);
		CP_ASSERT(map_.empty());
	}

	void AssetManager::set_assets_path(const std::string& path)
	{
		assets_path_ = path;
	}

	void AssetManager::set_cache_path(const std::string& path)
	{
		cache_path_ = path;
	}

	void AssetManager::destroy_entry(AssetEntry& entry)
	{
		mutex_.lock();
		auto it = map_.find(entry.get_id_hash());
		map_.erase(it);
		mutex_.unlock();
	}

	void AssetManager::add_request(const UntypedAssetHandle& request)
	{
		std::scoped_lock lock(mutex_);
		requests_.push(request);
	}

	void AssetManager::process_requests()
	{
		for (;;)
		{
			UntypedAssetHandle handle;
			{
				std::scoped_lock lock(mutex_);
				if (requests_.empty())
					break;
				handle = requests_.back();
				requests_.pop();
			}
			AssetEntry* entry = handle.entry_;
			entry->update_async();
		}
	}

	void AssetManager::on_entry_updated(AssetEntry& entry)
	{
		process_requests();
	}

	MappedAssetData AssetManager::map_asset(AssetEntry& entry)
	{
		std::scoped_lock lock(mutex_);
		for (AssetProvider* provider : providers_)
		{
			MappedAssetData data = provider->map_asset(entry);
			if (data.data() != nullptr)
				return std::move(data);
		}
		return MappedAssetData();
	}

	void AssetManager::register_provider(AssetProvider& provider)
	{
		std::scoped_lock lock(mutex_);
		CP_ASSERT(!contains(providers_, &provider));
		providers_.push_back(&provider);
	}

	void AssetManager::unregister_provider(AssetProvider& provider)
	{
		std::scoped_lock lock(mutex_);
		erase_first(providers_, &provider);
	}

	AssetEntry* AssetManager::get_or_create_entry(const std::string& id, const Type& type)
	{
		const uint64 id_hash = hash::resource_id::hash64(id);
		std::scoped_lock lock(mutex_);
		auto it = map_.find(id_hash);
		if (it == map_.end())
		{
			AssetEntry* entry = new AssetEntry(id, id_hash, type);
			map_[id_hash] = entry;
			return entry;
		}
		else
		{
			return it->second;
		}
	}
}
