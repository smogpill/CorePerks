// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/base/resource_base.h"
#include "core_perks/io/resources/resource_handle.h"
#include "core_perks/io/resources/providers/mapped_resource_data.h"
#include "core_perks/patterns/reference.h"
#include "core_perks/patterns/hashed_string.h"

#if 0

namespace cp
{
	class ResourceLoader; class Resource; class ResourceGenerator;

	class ResourceEntry : public RefCounted
	{
		using Base = RefCounted;
	public:
		using Callback = std::function<void(ResourceEntry&)>;
		ResourceEntry(const HashedString& id, const Type& type);
		virtual ~ResourceEntry();

		const HashedString& get_id() const { return id_; }
		std::string get_name() const;
		void add_loading_dependency();
		void remove_loading_dependency();
		//void add_load_request(ResourceHandle& handle);
		//void remove_load_request(ResourceHandle& handle);
		void load_async(Callback callback);
		bool path_exists() const;
		void unload_async();
		void store_async(Callback callback);
		RefPtr<Resource> get() const { return resource_; }
		void set(const RefPtr<Resource>& resource);
		std::string get_resource_path() const;
		bool is_ready() const { return state_ == ResourceState::READY; }
		void update_async();
		MappedResourceData get_mapped_data();
		const Type* get_type() const { return type_; }

	private:
		friend class ResourceLoader;
		friend class ResourceManager;
		void on_all_refs_removed() override;
		void add_load_callback(Callback callback);
		void on_update_async();
		void on_load_dependencies();
		void on_load();
		void on_unload();
		void on_dependency_loaded(ResourceEntry& entry);
		void flush_load_callbacks();
		void queue_async_callback(Callback&& callback);
		Resource* create_resource();

		HashedString id_;
		ResourcePriority priority_;
		const Type* type_ = nullptr;
		RefPtr<ResourceEntry> loading_parent_;
		std::atomic<uint32> nb_loading_dependencies_ = 0;
		std::mutex callback_mutex_;
		std::atomic<ResourceState> state_ = ResourceState::NONE;
		std::vector<ResourceHandle*> load_requests_;
		std::atomic<uint32> load_refs_ = 0;
		std::vector<Callback> load_callbacks_;
		RefPtr<Resource> resource_;
		std::atomic<Resource*> loading_resource_ = nullptr;
		bool loading_result_ = false;
		bool locked_ = false;
		std::mutex mutex_;
	};
}

#endif