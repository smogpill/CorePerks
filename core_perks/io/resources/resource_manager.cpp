// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/io/resources/resource_manager.h"
#include "core_perks/io/resources/resource_loader.h"

namespace cp
{
	ResourceManager::~ResourceManager()
	{
		std::scoped_lock lock(mutex_);
		CP_ASSERT(map_.empty());
	}

	void ResourceManager::set_assets_path(const std::string& path)
	{
		assets_path_ = path;
	}

	void ResourceManager::set_cache_path(const std::string& path)
	{
		cache_path_ = path;
	}

	void ResourceManager::destroy_entry(ResourceEntry& entry)
	{
		mutex_.lock();
		auto it = map_.find(entry.get_id_hash());
		map_.erase(it);
		mutex_.unlock();
	}

	void ResourceManager::add_request(const UntypedResourceHandle& request)
	{
		std::scoped_lock lock(mutex_);
		requests_.push(request);
	}

	ResourceEntry* ResourceManager::get_or_create_entry(const std::string& id, const Type& type)
	{
		const uint64 id_hash = hash::resource_id::hash64(id);
		std::scoped_lock lock(mutex_);
		auto it = map_.find(id_hash);
		if (it == map_.end())
		{
			ResourceEntry* entry = new ResourceEntry(id, id_hash, type);
			map_[id] = entry;
			return entry;
		}
		else
		{
			return it->second;
		}
	}
}
