// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/resources/resource_handle.h"
#include "core_perks/io/resources/resource_manager.h"
#include "core_perks/io/resources/resource_entry.h"
#include "core_perks/io/streams/binary_output_stream.h"
#include "core_perks/io/streams/binary_input_stream.h"

namespace cp
{
	ResourceHandleBase::ResourceHandleBase(Resource* resource)
		: resource_(resource)
	{
	}

	ResourceHandle::ResourceHandle(const HashedString& id, const Type& type)
		: entry_(ResourceManager::get().get_or_create_entry(id, type))
	{
	}

	void ResourceHandle::set(const HashedString& id, const Type& type)
	{
		entry_ = ResourceManager::get().get_or_create_entry(id, type);
	}

	void ResourceHandle::unload_async()
	{
		if (entry_)
			entry_->unload_async();
	}

	void ResourceHandle::store_async(std::function<void(ResourceEntry&)> on_done)
	{
		if (entry_)
			entry_->store_async(std::move(on_done));
	}

	void ResourceHandle::load_async(std::function<void(ResourceEntry&)> on_done)
	{
		if (entry_)
			entry_->load_async(std::move(on_done));
	}

	std::string ResourceHandle::get_name() const
	{
		return entry_ ? entry_->get_name() : nullptr;
	}

	const HashedString& ResourceHandle::get_id() const
	{
		return entry_ ? entry_->get_id() : HashedString::get_empty();
	}

	Resource* ResourceHandle::get() const
	{
		return entry_ ? entry_->get() : nullptr;
	}

	BinaryInputStream& operator>>(BinaryInputStream& stream, ResourceHandle& handle)
	{
		std::string id;
		stream >> id;
		uint32 type_hash = 0;
		stream >> type_hash;
		const Type* type = Type::get_by_name_hash(type_hash);
		if (type)
			handle.set(id, *type);
		return stream;
	}

	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const ResourceHandle& handle)
	{
		const HashedString& id = handle.entry_ ? handle.entry_->get_id() : HashedString::get_empty();
		stream << id;
		const uint32 type_hash = handle.entry_ ? handle.entry_->get_type()->get_name_hash() : 0;
		stream << type_hash;
		return stream;
	}
}
