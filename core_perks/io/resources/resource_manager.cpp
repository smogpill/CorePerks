// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/io/resources/resource_manager.h"
#include "core_perks/io/resources/resource_loader.h"

namespace cp
{
	ResourceManager::ResourceManager(const std::string& assetsPath)
		: _assets_path(assetsPath)
	{

	}

	ResourceManager::~ResourceManager()
	{
		std::scoped_lock lock(_mutex);
		CP_ASSERT(_map.empty());
	}

	void ResourceManager::destroy_holder(ResourceHolder& holder)
	{
		_mutex.lock();
		auto it = _map.find(holder.get_id());
		_map.erase(it);
		_mutex.unlock();
	}

	ResourceHolder* ResourceManager::get_or_create_holder(const std::string& id)
	{
		std::scoped_lock lock(_mutex);
		auto it = _map.find(id);
		if (it == _map.end())
		{
			ResourceHolder* holder = new ResourceHolder(id);
			_map[id] = holder;
			return holder;
		}
		else
		{
			return it->second;
		}
	}
}
