// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/resources/providers/folder_resource_provider.h"
#include "core_perks/io/resources/resource_id.h"
#include "core_perks/io/resources/resource.h"
#include "core_perks/io/file/mapped_file_region.h"
#include "core_perks/io/streams/binary_output_stream.h"
#include "core_perks/io/streams/binary_serializer.h"

namespace cp
{
	FolderResourceProvider::FolderResourceProvider(const std::string& path)
		: folder_path_(path)
	{
	}

	bool FolderResourceProvider::has_resource(const ResourceID& id) const
	{
		return std::filesystem::exists(get_resource_path(id));
	}

	ResourceMapping FolderResourceProvider::map_resource(const ResourceID& id)
	{
		const std::filesystem::path path = get_resource_path(id);
		ResourceMapping mapping;
		if (!std::filesystem::exists(path))
		{
			mapping.status_ = ResourceMapping::Status::MISSING;
			return mapping;
		}

		RefPtr<FileHandle> file(new FileHandle(path.string(), FileHandle::Mode::READ));
		if (!file->is_open())
		{
			mapping.status_ = ResourceMapping::Status::FAILED;
			return mapping;
		}
		
		MappedFileRegion region(file);
		if (!region.is_mapped())
		{
			mapping.status_ = ResourceMapping::Status::FAILED;
			return mapping;
		}
		mapping.region_ = std::move(region);
		mapping.status_ = ResourceMapping::Status::SUCCESS;
		return mapping;
	}

	void FolderResourceProvider::store_resource_async(const ResourceID& id, const RefPtr<Resource>& resource, std::function<void(bool)>&& on_done)
	{
		OutputMemoryBuffer buffer;
		OutputBinarySerializer serializer(buffer);
		resource->serialize(serializer);
		if (serializer.failed())
		{
			on_done(false);
			return;
		}

		const std::filesystem::path path = get_resource_path(id);
		FileHandle file(path.string(), FileHandle::Mode::WRITE_TRUNCATE);
		if (!file.is_open())
		{
			on_done(false);
			return;
		}
		if (!buffer.write_to_file(file))
		{
			on_done(false);
			return;
		}
		
		on_done(true);
	}

	std::filesystem::path FolderResourceProvider::get_resource_path(const ResourceID& id) const
	{
		return folder_path_ / id.string();
	}
}
