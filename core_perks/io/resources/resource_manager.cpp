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

	ResourceHandle ResourceManager::load_async(const HashedString& id, const Type& type, std::function<void(Resource&)> on_done)
	{
		std::scoped_lock lock(mutex_);
		Resource* resource = get_resource_no_lock(id, type);
		if (!resource)
		{
			resource = type.create<Resource>();
			resource->id_ = id;
			map_[id.hash()] = resource;
			LoadRequest request;
			request.handle_ = resource;
			request.on_done_ = on_done;
			load_requests_.push(std::move(request));
		}
		return ResourceHandle(resource);
	}

	ResourceHandle ResourceManager::set(const HashedString& id, const cp::RefPtr<Resource>& resource)
	{
		std::scoped_lock lock(mutex_);
		Resource* old_resource = get_resource(id.hash());
		if (old_resource)
			old_resource->id_.clear();
		map_[id.hash()] = resource.get();
		if (!resource)
			return ResourceHandle();
		resource->id_ = id;
		return ResourceHandle(resource.get());
	}

	void ResourceManager::remove_resource(Resource& resource)
	{
		std::scoped_lock lock(mutex_);
		auto it = map_.find(resource.get_id().hash());
		map_.erase(it);
	}

	void ResourceManager::add_request(const ResourceHandle& request)
	{
		std::scoped_lock lock(mutex_);
		load_requests_.push(request);
	}

	void ResourceManager::process_requests()
	{
		for (;;)
		{
			ResourceHandle handle;
			{
				std::scoped_lock lock(mutex_);
				if (load_requests_.empty())
					break;
				handle = load_requests_.back();
				load_requests_.pop();
			}
			if (handle)
				handle->update_async();
		}
	}

	void ResourceManager::on_resource_updated(Resource& resource)
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

	Resource* ResourceManager::get_resource_no_lock(const HashedString& id, const Type& type) const
	{
		auto it = map_.find(id.hash());
		if (it != map_.end())
			return it->second;
		return nullptr;
	}

	Resource* ResourceManager::get_resource(const HashedString& id, const Type& type) const
	{
		std::scoped_lock lock(mutex_);
		return get_resource_no_lock(id, type);
	}
}
