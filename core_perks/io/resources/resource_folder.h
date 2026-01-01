// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/resource.h"
#include "core_perks/io/file.h"

namespace cp
{
	class ResourceEntry;
	class ResourceHandle;

	class ResourceFolder : public Resource
	{
		CP_BASE(Resource);
		CP_CLASS(ResourceFolder);
	public:
		ResourceFolder(const std::string& path);
		~ResourceFolder() override;
		MappedResourceData map_sub_resource(const ResourceHandle& resource) override;
		void unmap_sub_resource(const ResourceHandle& resource) override;

		void store_sub_resource_async(const ResourceHandle& resource);

	protected:
		bool on_ready() override;
		void on_unready() override;

	private:
		struct ResourceInfo
		{
			FileHandle file_;
			uint64 resource_id_hash_ = 0;
		};

		ResourceInfo& get_or_open_file(const ResourceHandle& resource);
		ResourceInfo* get_file(const ResourceHandle& resource) const;

		std::filesystem::path path_;
		std::vector<ResourceInfo*> open_files_;
	};
}
