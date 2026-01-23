// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/resources/resource_manager.h"
#include "core_perks/io/resources/resource_entry.h"
#include "core_perks/io/resources/providers/resource_provider.h"
#include "core_perks/containers/vector_extensions.h"
#include "core_perks/threading/job/job_system.h"

namespace cp
{
	ResourceManager::~ResourceManager()
	{
		std::scoped_lock lock(mutex_);
		CP_ASSERT(map_.empty());
	}

	ResourceHandle ResourceManager::get_handle(const ResourceID& id) const
	{
		std::scoped_lock lock(mutex_);
		return ResourceHandle(get_entry_no_lock(id));
	}

	ResourceEntry* ResourceManager::get_or_create_entry(const ResourceID& id, const Type& type)
	{
		std::scoped_lock lock(mutex_);
		ResourceEntry* entry = get_entry_no_lock(id);
		if (entry)
		{
			if (&entry->get_type() != &type)
			{
				CP_ERROR("{}: Invalid requested type: {} instead of {}", id.string(), type.get_name(), entry->get_type().get_name());
				entry = nullptr;
			}
		}
		else
		{
			entry = new ResourceEntry(id, type);
			map_[entry->get_id()] = entry;
		}
		
		return entry;
	}

	void ResourceManager::remove_resource(Resource& resource)
	{
		std::scoped_lock lock(mutex_);
		auto it = map_.find(resource.get_id());
		map_.erase(it);
	}

	void ResourceManager::push_load_request(LoadRequest&& request)
	{
		{
			std::scoped_lock lock(mutex_);
			load_requests_.emplace(std::move(request));
		}
		process_requests();
	}

	void ResourceManager::process_requests()
	{
		for (;;)
		{
			LoadRequest request;

			{
				std::scoped_lock lock(mutex_);
				if (load_requests_.empty())
					break;
				request = load_requests_.back();
				load_requests_.pop();
			}

			request.handle_.entry_->notify_ready_to_load();
		}
	}

	void ResourceManager::on_resource_updated(Resource& resource)
	{
		process_requests();
	}

	ResourceMapping ResourceManager::map_resource(const ResourceID& id)
	{
		ResourceMapping mapping;
		std::scoped_lock lock(mutex_);
		for (ResourceProvider* provider : providers_ | std::views::reverse)
		{
			mapping = provider->map_resource(id);
			if (mapping.status_ != ResourceMapping::Status::MISSING)
				break;
		}
		return mapping;
	}

	bool ResourceManager::exists_in_storage(const ResourceID& id) const
	{
		std::scoped_lock lock(mutex_);
		for (ResourceProvider* provider : providers_ | std::views::reverse)
		{
			if (provider->has_resource(id))
				return true;
		}
		return false;
	}

	void ResourceManager::register_provider(ResourceProvider& provider)
	{
		std::scoped_lock lock(mutex_);
		CP_ASSERT(!contains(providers_, &provider));
		providers_.push_back(&provider);
	}

	void ResourceManager::unregister_provider(ResourceProvider& provider)
	{
		std::scoped_lock lock(mutex_);
		erase_first(providers_, &provider);
	}

	void ResourceManager::on_entry_destroyed(ResourceEntry& entry)
	{
		std::scoped_lock lock(mutex_);
		map_.erase(entry.get_id());
	}

	ResourceEntry* ResourceManager::get_entry_no_lock(const ResourceID& id) const
	{
		auto it = map_.find(id);
		if (it != map_.end())
			return it->second;
		return nullptr;
	}
}
