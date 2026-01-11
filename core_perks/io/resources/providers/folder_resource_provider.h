// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/providers/resource_provider.h"
#include "core_perks/io/file/file_handle.h"

namespace cp
{
	class FolderResourceProvider : public ResourceProvider
	{
	public:
		FolderResourceProvider(const std::string& path);

		bool has_resource(const ResourceID& id) const override;
		ResourceMapping map_resource(const ResourceID& id) override;

	private:
		std::filesystem::path get_resource_path(const ResourceID& id) const;

		std::filesystem::path folder_path_;
	};
}
