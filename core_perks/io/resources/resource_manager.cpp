// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/resources/resource_manager.h"
#include "core_perks/io/resources/resource_loader.h"
#include "core_perks/io/resources/resource_entry.h"
#include "core_perks/containers/vector_extensions.h"

namespace cp
{
	ResourceManager::~ResourceManager()
	{
		std::scoped_lock lock(mutex_);
		CP_ASSERT(map_.empty());
	}

	ResourceHandle ResourceManager::create(const HashedString& id, const Type& type)
	{
		std::scoped_lock lock(mutex_);
		CP_ASSERT(false);
		return ResourceHandle();
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
		auto it = map_.find(entry.get_id().hash());
		map_.erase(it);
		mutex_.unlock();
	}

	void ResourceManager::add_request(const ResourceHandle& request)
	{
		std::scoped_lock lock(mutex_);
		requests_.push(request);
	}

	void ResourceManager::process_requests()
	{
		for (;;)
		{
			ResourceHandle handle;
			{
				std::scoped_lock lock(mutex_);
				if (requests_.empty())
					break;
				handle = requests_.back();
				requests_.pop();
			}
			if (handle.entry_)
				handle.entry_->update_async();
		}
	}

	void ResourceManager::on_entry_updated(ResourceEntry& entry)
	{
		process_requests();
	}

	MappedResourceData ResourceManager::map_resource(const ResourceHandle& resource)
	{
		std::scoped_lock lock(mutex_);
		for (Resource* provider : providers_ | std::views::reverse)
		{
			MappedResourceData data = provider->map_sub_resource(resource);
			if (data.data() != nullptr)
				return std::move(data);
		}
		return MappedResourceData(resource);
	}

	void ResourceManager::register_provider(Resource& provider)
	{
		std::scoped_lock lock(mutex_);
		CP_ASSERT(!contains(providers_, &provider));
		providers_.push_back(&provider);
	}

	void ResourceManager::unregister_provider(Resource& provider)
	{
		std::scoped_lock lock(mutex_);
		erase_first(providers_, &provider);
	}

	ResourceEntry* ResourceManager::get_or_create_entry(const HashedString& id, const Type& type)
	{
		std::scoped_lock lock(mutex_);
		auto it = map_.find(id.hash());
		if (it == map_.end())
		{
			ResourceEntry* entry = new ResourceEntry(id, type);
			map_[id.hash()] = entry;
			return entry;
		}
		else
		{
			return it->second;
		}
	}
}
