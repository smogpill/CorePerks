// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/singleton.h"
#include "core_perks/io/resources/resource.h"
#include "core_perks/io/resources/resource_entry.h"
#include "core_perks/io/resources/resource_handle.h"
#include "core_perks/io/resources/providers/mapped_resource_data.h"

namespace cp
{
	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		~ResourceManager();
	
		ResourceHandle set_resource(const HashedString& id, const RefPtr<Resource>& resource);
		ResourceHandle load_async(const HashedString& id, const Type& type, std::function<void(Resource&)> on_done = [](Resource&){});
		template <class T>
		ResourceHandle load_async(const HashedString& id, std::function<void(Resource&)> on_done = [](Resource&) {}) { return load_async(id, T::get_type_static(), on_done); }

		// Assets path
		void set_assets_path(const std::string& path);
		const std::string& get_assets_path() const { return assets_path_; }

		// Cache path
		void set_cache_path(const std::string& path);
		const std::string& get_cache_path() const { return cache_path_; }

		void register_provider(Resource& resource);
		void unregister_provider(Resource& resource);

	private:
		friend class ResourceEntry;
		friend class Resource;
		friend class ResourceHandle;

		ResourceEntry* get_or_create_entry(const HashedString& id, const Type& type);
		void destroy_entry(ResourceEntry& entry);
		void add_request(const ResourceHandle& request);
		void process_requests();
		void on_entry_updated(ResourceEntry& entry);
		MappedResourceData map_resource(const ResourceHandle& resource);

		mutable std::mutex mutex_;
		std::unordered_map<uint64, Resource*> map_;
		std::queue<ResourceHandle> requests_;
		std::string assets_path_;
		std::string cache_path_;
		std::vector<Resource*> providers_;
		uint max_parallel_updates_ = 4;

		/*
		Streaming
		std::queue<Request> immediate_queue_; // Must load now (player weapon, current mission)
		std::queue<Request> high_priority_queue_; // Visible/nearby
		std::queue<Request> low_priority_queue_; // Predictive/prefetch
		std::queue<Request> unload_queue_;
		*/
	};
}
