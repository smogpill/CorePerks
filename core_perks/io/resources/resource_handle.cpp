// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/io/resources/resource_handle.h"
#include "core_perks/io/resources/resource_manager.h"

namespace cp
{
	UntypedResourceHandle::UntypedResourceHandle(const std::string& id, const Type& type)
		: entry_(ResourceManager::get().get_or_create_entry(id, type))
	{
	}

	void UntypedResourceHandle::set_id(const std::string& id, const Type& type)
	{
		entry_ = ResourceManager::get().get_or_create_entry(id, type);
	}

	void UntypedResourceHandle::set_resource(Resource* resource)
	{
		if (entry_)
		{
			entry_->set(resource);
		}
		else
		{
			delete resource;
		}
	}

	void UntypedResourceHandle::unload_async()
	{
		if (entry_)
			entry_->unload_async();
	}

	void UntypedResourceHandle::store_async(std::function<void(bool)> on_done)
	{
		if (entry_)
			entry_->store_async(std::move(on_done));
	}

	void UntypedResourceHandle::load_async(std::function<void(bool)> on_done)
	{
		if (entry_)
			entry_->load_async(std::move(on_done));
	}
}
