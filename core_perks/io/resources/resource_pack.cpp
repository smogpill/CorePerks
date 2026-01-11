// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/resources/resource_pack.h"
#include "core_perks/io/resources/resource_manager.h"
#include "core_perks/containers/vector_extensions.h"

#if 0

namespace cp
{
	CP_DEFINE_CLASS(ResourcePack)
	{
	}

	void operator>>(BinaryInputStream& stream, ResourcePack::SubResourceInfo& entry)
	{
		stream >> entry.id_;
		stream >> entry.offset_;
		stream >> entry.size_;
	}

	bool ResourcePack::on_load()
	{
		CP_TRY(Base::on_load());
		ResourceEntry& entry = get_entry();
		MappedResourceData data = entry.get_mapped_data();
		BinaryInputStream stream = data.get_stream();
		stream >> sub_resources_;
		ResourceManager::get().register_provider(*this);
		return true;
	}

	void ResourcePack::on_unload()
	{
		ResourceManager::get().unregister_provider(*this);
		sub_resources_.clear();
		Base::on_unload();
	}

	ResourceMapping ResourcePack::map_sub_resource(const ResourceID& id)
	{
		SubResourceInfo* info = get_sub_resource_info(resource);
		if (info == nullptr)
			return ResourceMapping();
		return MappedFileRegion(file_handle_, info->offset_, info->size_, FileHandle::Access::READ_ONLY);
	}

	ResourcePack::SubResourceInfo* ResourcePack::get_sub_resource_info(const ResourceHandle& resource)
	{
		const HashedString& id = resource.get_id();
		for (SubResourceInfo& info : sub_resources_)
		{
			if (info.id_ == id)
				return &info;
		}
		return nullptr;
	}
}

#endif