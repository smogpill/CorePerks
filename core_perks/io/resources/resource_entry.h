// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2026 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/base/resource_base.h"
#include "core_perks/io/file/mapped_file_region.h"
#include "core_perks/patterns/reference.h"

namespace cp
{
	class Resource; class ResourceManager;

	class ResourceEntry : public RefCounted
	{
	public:
		ResourceEntry(const ResourceID& id);
		~ResourceEntry();

		void set(const RefPtr<Resource>& resource);
		void load_async(std::function<void()>&& on_done = [](){});
		template <class T = Resource>
		RefPtr<T> get() const { return resource_; }
		const ResourceID& get_id() const { return id_; }
		bool is_ready() const { return state_ == ResourceState::READY; }

	protected:
		void on_all_refs_removed() override;

	private:
		friend class ResourceManager;

		ResourceManager& manager();
		void update_state();
		void do_loading();
		void do_readying();
		void on_dependency_loading_done(ResourceEntry& dependency);
		void on_all_dependencies_loading_done();
		void flush_loading_callbacks();
		void notify_ready_to_load();

		std::mutex mutex_;
		ResourceState state_ = ResourceState::NONE;
		const Type* type_ = nullptr;
		ResourceID id_;
		RefPtr<Resource> resource_;
		RefPtr<Resource> loading_resource_;
		std::vector<std::function<void()>> loading_done_callbacks_;
		std::atomic<uint32> waiting_dependencies_count_ = 0;
	};

	template <class T>
	class ResourceEntryT : public ResourceEntry
	{
		CP_BASE(ResourceEntry);
	public:
		using ResourceEntry::ResourceEntry;

		RefPtr<T> get() const { return Base::get<T>(); }
	};
}
