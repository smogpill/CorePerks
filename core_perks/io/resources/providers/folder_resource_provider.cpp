// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/resources/providers/folder_resource_provider.h"
#include "core_perks/io/resources/resource_id.h"
#include "core_perks/io/file/mapped_file_region.h"

namespace cp
{
	FolderResourceProvider::FolderResourceProvider(const std::string& path)
		: folder_path_(path)
	{
	}

	MappedFileRegion FolderResourceProvider::map_resource(const ResourceID& id)
	{
		const std::filesystem::path asset_path = folder_path_ / id.string();
		RefPtr<FileHandle> file(new FileHandle(asset_path.string(), FileHandle::Mode::READ));
		return MappedFileRegion(file);
	}
}
