// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/resource.h"
#include "core_perks/io/resources/resource_handle.h"
#include "core_perks/io/file/file_handle.h"
#include "core_perks/patterns/hashed_string.h"

#if 0

namespace cp
{
	class ResourcePack : public Resource
	{
		CP_BASE(Resource);
		CP_CLASS(ResourcePack);
	public:
		struct SubResourceInfo
		{
			HashedString id_;
			uint64 offset_ = 0;
			uint64 size_ = 0;
		};

		virtual ResourceMapping map_sub_resource(const ResourceID& id) override;

		void build(const std::vector<ResourceHandle>& resources);

	protected:
		void on_serialize(BinarySerializer& serializer) override;
		bool on_load() override;
		void on_unload() override;

	private:
		SubResourceInfo* get_sub_resource_info(const ResourceHandle& resource);

		std::vector<SubResourceInfo> sub_resources_;
		//std::unorder_map<uint64, SubResourceInfo*> id_hash_to_sub_resource_;
		RefPtr<FileHandle> file_handle_;
	};
}

#endif