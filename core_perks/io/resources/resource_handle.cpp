// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/io/resources/resource_handle.h"
#include "core_perks/io/resources/resource_manager.h"

namespace cp
{
	UntypedResourceHandle::UntypedResourceHandle(const std::string& id)
		: _entry(ResourceManager::get().get_or_create_entry(id))
	{
	}

	void UntypedResourceHandle::set_id(const std::string& id)
	{
		_entry = ResourceManager::get().get_or_create_entry(id);
	}

	void UntypedResourceHandle::set_resource(Resource* resource)
	{
		if (_entry)
		{
			_entry->set(resource);
		}
		else
		{
			delete resource;
		}
	}

	void UntypedResourceHandle::unload_async()
	{
		if (_entry)
			_entry->unload_async();
	}

	void UntypedResourceHandle::store_async(std::function<void(bool)> on_done)
	{
		if (_entry)
			_entry->store_async(std::move(on_done));
	}

	void UntypedResourceHandle::load_async(std::function<void(bool)> on_done)
	{
		if (_entry)
			_entry->load_async(std::move(on_done));
	}
}
