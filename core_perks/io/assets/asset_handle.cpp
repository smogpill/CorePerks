// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/assets/asset_handle.h"
#include "core_perks/io/assets/asset_manager.h"
#include "core_perks/io/assets/asset_entry.h"
#include "core_perks/io/streams/binary_output_stream.h"
#include "core_perks/io/streams/binary_input_stream.h"

namespace cp
{
	AssetHandle::AssetHandle(const std::string& id, const Type& type)
		: entry_(AssetManager::get().get_or_create_entry(id, type))
	{
	}

	AssetHandle::AssetHandle(AssetEntry* entry)
		: entry_(entry)
	{
	}

	void AssetHandle::set_id(const std::string& id, const Type& type)
	{
		entry_ = AssetManager::get().get_or_create_entry(id, type);
	}

	void AssetHandle::set_resource(Asset* resource)
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

	void AssetHandle::unload_async()
	{
		if (entry_)
			entry_->unload_async();
	}

	void AssetHandle::store_async(std::function<void(bool)> on_done)
	{
		if (entry_)
			entry_->store_async(std::move(on_done));
	}

	void AssetHandle::load_async(std::function<void(bool)> on_done)
	{
		if (entry_)
			entry_->load_async(std::move(on_done));
	}

	std::string AssetHandle::get_name() const
	{
		return entry_ ? entry_->get_name() : nullptr;
	}

	const std::string& AssetHandle::get_id() const
	{
		static const std::string empty_string;
		return entry_ ? entry_->get_id() : empty_string;
	}

	Asset* AssetHandle::get() const
	{
		return entry_ ? entry_->get() : nullptr;
	}

	BinaryInputStream& operator>>(BinaryInputStream& stream, AssetHandle& handle)
	{
		std::string id;
		stream >> id;
		uint32 type_hash = 0;
		stream >> type_hash;
		const Type* type = Type::get_by_name_hash(type_hash);
		if (type)
			handle.set_id(id, *type);
		return stream;
	}

	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const AssetHandle& handle)
	{
		static const std::string empty_string;
		const std::string& id = handle.entry_ ? handle.entry_->get_id() : empty_string;
		stream << id;
		const uint32 type_hash = handle.entry_ ? handle.entry_->get_type()->get_name_hash() : 0;
		stream << type_hash;
		return stream;
	}
}
