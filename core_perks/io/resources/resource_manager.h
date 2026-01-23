// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/singleton.h"
#include "core_perks/io/resources/resource.h"
#include "core_perks/io/resources/resource_entry.h"
#include "core_perks/io/resources/resource_handle.h"
#include "core_perks/io/resources/providers/resource_mapping.h"

namespace cp
{
	class ResourceProvider;

	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		~ResourceManager();
	
		ResourceHandle get_handle(const ResourceID& id) const;

		void register_provider(ResourceProvider& provider);
		void unregister_provider(ResourceProvider& provider);

	private:
		friend class Resource;
		friend class ResourceHandle;
		friend class ResourceEntry;

		struct LoadRequest
		{
			ResourceHandle handle_;
		};

		struct Hash
		{
			std::size_t operator()(const ResourceID& id) const
			{
				return id.hash();
			}
		};
		
		ResourceEntry* get_or_create_entry(const ResourceID& id, const Type& type);
		ResourceEntry* get_entry_no_lock(const ResourceID& id) const;
		void on_entry_destroyed(ResourceEntry& entry);
		void remove_resource(Resource& resource);
		void push_load_request(LoadRequest&& request);
		void process_requests();
		void on_resource_updated(Resource& resource);
		ResourceMapping map_resource(const ResourceID& id);
		bool exists_in_storage(const ResourceID& id) const;

		mutable std::mutex mutex_;
		std::unordered_map<ResourceID, ResourceEntry*, Hash> map_;
		std::queue<LoadRequest> load_requests_;
		std::vector<ResourceProvider*> providers_;
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
