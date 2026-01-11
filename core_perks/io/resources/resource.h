// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/base/resource_base.h"
#include "core_perks/io/resources/providers/resource_mapping.h"
#include "core_perks/io/resources/resource_handle.h"
#include "core_perks/patterns/hashed_string.h"
#include "core_perks/patterns/reference.h"

namespace cp
{
	class BinaryOutputStream; class MappedResourceData; class BinarySerializer; class ResourceID;

	class Resource : public RefCounted
	{
		CP_BASE(RefCounted);
		CP_CLASS(Resource);

	public:
		virtual ~Resource() = default;

		const ResourceID& get_id() const;
		ResourceHandle get_handle() const;
		bool is_ready() const { return state_ == ResourceState::READY; }

		// Dependencies
		void add_dependency(const ResourceHandle& handle);
		const std::vector<ResourceHandle>& get_dependencies() const { return dependencies_; }

		// Sub resources
		virtual ResourceMapping map_sub_resource(const ResourceID& id) { return ResourceMapping(); }
		virtual void store_sub_resource_async(const ResourceHandle& resource, std::function<void(bool)> on_done = [](bool) {}) { on_done(false); }

		virtual bool should_load_file() const { return true; }

	protected:
		virtual bool on_read(const MappedFileRegion& region) { return true; }
		virtual void on_serialize(BinarySerializer& serializer);
		//virtual void on_serialize_post_dependencies(BinarySerializer& serializer) {}
		virtual bool on_load();
		virtual void on_unload();
		virtual bool on_dependency_loaded(Resource& dependency) { return true; }
		virtual bool on_ready() { return true; }
		virtual void on_unready() {}

	private:
		friend class ResourceManager;
		friend class ResourceEntry;

		std::mutex mutex_;
		ResourceEntry* entry_ = nullptr;
		uint32 version_ = 0;
		ResourceState state_ = ResourceState::NONE;
		std::vector<ResourceHandle> dependencies_;
		std::vector<Resource*> dependents_;
		std::vector<ResourceHandle*> users_;
	};
}
