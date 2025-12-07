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
		std::scoped_lock lock(_mutex);
		CP_ASSERT(_map.empty());
	}

	void ResourceManager::set_assets_path(const std::string& path)
	{
		_assets_path = path;
	}

	void ResourceManager::set_cache_path(const std::string& path)
	{
		_cache_path = path;
	}

	void ResourceManager::destroy_entry(ResourceEntry& entry)
	{
		_mutex.lock();
		auto it = _map.find(entry.get_id());
		_map.erase(it);
		_mutex.unlock();
	}

	void ResourceManager::add_request(const UntypedResourceHandle& request)
	{
		std::scoped_lock lock(_mutex);
		_requests.push(request);
	}

	ResourceEntry* ResourceManager::get_or_create_entry(const std::string& id)
	{
		const uint64 id_hash = hash_resource_id(id);
		std::scoped_lock lock(_mutex);
		auto it = _map.find(id_hash);
		if (it == _map.end())
		{
			ResourceEntry* entry = new ResourceEntry(id, id_hash);
			_map[id] = entry;
			return entry;
		}
		else
		{
			return it->second;
		}
	}
}
