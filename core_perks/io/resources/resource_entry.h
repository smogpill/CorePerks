// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2026 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/resource_id.h"
#include "core_perks/io/file/mapped_file_region.h"
#include "core_perks/patterns/reference.h"

namespace cp
{
	class Resource; class ResourceManager;

	class ResourceEntry : public RefCounted
	{
	public:
		ResourceEntry(const ResourceID& id, const Type& type);
		~ResourceEntry();

		const Type& get_type() const { CP_ASSERT(type_); return *type_; }
		void create();
		void load_async(std::function<void()>&& on_done = [](){});
		RefPtr<Resource> get() const { return resource_; }
		const ResourceID& get_id() const { return id_; }
		ResourceState get_state() const { return state_; }
		bool missing() const { return state_ == ResourceState::MISSING; }
		bool ready() const { return state_ == ResourceState::READY; }
		bool failed() const { return state_ == ResourceState::FAILED; }

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
		void finish_loading();

		std::mutex mutex_;
		ResourceState state_ = ResourceState::NONE;
		const Type* type_ = nullptr;
		ResourceID id_;
		RefPtr<Resource> resource_;
		RefPtr<Resource> loading_resource_;
		std::vector<std::function<void()>> loading_done_callbacks_;
		std::atomic<uint32> waiting_dependencies_count_ = 0;
	};
}
