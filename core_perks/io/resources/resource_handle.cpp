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
	ResourceHandle::ResourceHandle(const ResourceID& id, const Type& type)
	{
		bind(id, type);
	}

	ResourceHandle::ResourceHandle(ResourceEntry* entry)
		: entry_(entry)
	{
	}

	/*
	const Type* ResourceHandle::get_type() const
	{
		return entry_ ? &entry_->get_type() : nullptr;
	}
	*/

	void ResourceHandle::bind(const ResourceID& id, const Type& type)
	{
		entry_ = ResourceManager::get().get_or_create_entry(id, type);
	}

	/*
	void ResourceHandle::create() const
	{
		CP_ASSERT(entry_);
		entry_->create();
	}

	void ResourceHandle::load_async(std::function<void()>&& on_done) const
	{
		CP_ASSERT(entry_);
		entry_->load_async(std::move(on_done));
	}

	const ResourceID& ResourceHandle::get_id() const
	{
		return entry_ ? entry_->get_id() : ResourceID::get_empty();
	}
	*/

	void ResourceHandle::reset()
	{
		entry_.reset();
	}

	BinaryInputStream& operator>>(BinaryInputStream& stream, ResourceHandle& handle)
	{
		Type* type;
		stream >> type;
		if (type)
		{
			ResourceID id;
			stream >> id;
			handle.bind(id, *type);
		}
		else
		{
			handle.reset();
		}
		return stream;
	}

	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const ResourceHandle& handle)
	{
		if (handle)
		{
			stream << &handle->get_type();
			stream << handle->get_id();
		}
		else
		{
			stream << (Type*)nullptr;
		}
		return stream;
	}
}
