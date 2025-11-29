// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/io/resources/resource_handle.h"
#include "core_perks/io/resources/resource_manager.h"

namespace cp
{
	ResourceHandleBase::ResourceHandleBase(const std::string& id)
		: _holder(ResourceManager::get().get_or_create_holder(id))
	{
	}

	void ResourceHandleBase::set_id(const std::string& id)
	{
		_holder = ResourceManager::get().get_or_create_holder(id);
	}

	void ResourceHandleBase::set_resource(Resource* resource)
	{
		if (_holder)
		{
			_holder->set(resource);
		}
		else
		{
			delete resource;
		}
	}

	void ResourceHandleBase::unload_async()
	{
		if (_holder)
			_holder->unload_async();
	}

	void ResourceHandleBase::store_async(std::function<void(bool)> onDone)
	{
		if (_holder)
			_holder->store_async(std::move(onDone));
	}

	void ResourceHandleBase::load_async(std::function<Resource* ()> createFunction)
	{
		if (_holder)
			_holder->load_async(std::move(createFunction));
	}
}
