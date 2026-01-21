// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/resources/resource_entry.h"
#include "core_perks/io/resources/resource_manager.h"
#include "core_perks/io/resources/resource_handle.h"
#include "core_perks/io/streams/binary_serializer.h"
#include "core_perks/io/file/file_handle.h"
#include "core_perks/patterns/scope_exit.h"
#include "core_perks/threading/job/job_system.h"

namespace cp
{
	ResourceEntry::ResourceEntry(const ResourceID& id, const Type& type)
		: id_(id)
		, type_(&type)
	{
	}

	ResourceEntry::~ResourceEntry()
	{
		manager().on_entry_destroyed(*this);
	}

	void ResourceEntry::set_async(RefPtr<Resource> resource, std::function<void()>&& on_done)
	{
		std::scoped_lock lock(mutex_);
		loading_resource_ = resource;
		if (loading_resource_)
		{
			loading_resource_->entry_ = this;
			state_ = ResourceState::WAITING_DEPENDENCIES;
			load_dependencies_async();
		}
	}

	void ResourceEntry::load_async(std::function<void()>&& on_done)
	{
		ResourceManager::LoadRequest request;
		{
			std::scoped_lock lock(mutex_);
			if (state_ == ResourceState::READY || state_ == ResourceState::FAILED)
			{
				on_done();
				return;
			}
			loading_done_callbacks_.emplace_back(std::move(on_done));
			request.handle_.entry_ = this;
		}
		
		manager().push_load_request(std::move(request));
	}

	void ResourceEntry::on_all_refs_removed()
	{
		// TODO
	}

	ResourceManager& ResourceEntry::manager()
	{
		return ResourceManager::get();
	}

	void ResourceEntry::update_state()
	{
	}

	void ResourceEntry::notify_ready_to_load()
	{
		std::scoped_lock lock(mutex_);
		if (state_ == ResourceState::NONE)
		{
			state_ = ResourceState::LOADING;
			JobSystem& job_system = JobSystem::get();
			job_system.enqueue([this]()
				{
					this->do_loading();
				});
		}
	}

	void ResourceEntry::do_loading()
	{
		CP_ASSERT(state_ == ResourceState::LOADING);

		loading_resource_ = type_->create<Resource>();
		loading_resource_->entry_ = this;
		if (loading_resource_->should_load_file())
		{
			ResourceMapping mapping = manager().map_resource(id_);
			if (mapping.status_ == ResourceMapping::Status::MISSING)
			{
				state_ = ResourceState::MISSING;
				flush_loading_callbacks();
				return;
			}

			if (mapping.status_ == ResourceMapping::Status::FAILED)
			{
				state_ = ResourceState::FAILED;
				flush_loading_callbacks();
				return;
			}

			if (!loading_resource_->on_read(mapping.region_))
			{
				state_ = ResourceState::FAILED;
				flush_loading_callbacks();
				return;
			}

			if (loading_resource_->should_serialize())
			{
				InputBinarySerializer serializer(mapping.region_.cdata(), mapping.region_.size());
				loading_resource_->on_serialize(serializer);
				if (serializer.failed())
				{
					state_ = ResourceState::FAILED;
					flush_loading_callbacks();
					return;
				}
			}
		}

		state_ = ResourceState::WAITING_DEPENDENCIES;
		load_dependencies_async();
	}

	void ResourceEntry::load_dependencies_async()
	{
		const auto& dependencies = loading_resource_->get_dependencies();
		if (dependencies.empty())
		{
			state_ = ResourceState::READYING;
			do_readying();
			return;
		}
		waiting_dependencies_count_ = (uint32)dependencies.size();
		ResourceHandle self(this);
		for (const ResourceHandle& dependency : dependencies)
		{
			auto callback = [=]()
				{
					self->on_dependency_loading_done(*dependency.entry_);
				};
			dependency->load_async(callback);
		}
	}

	void ResourceEntry::do_readying()
	{
		CP_ASSERT(state_ == ResourceState::READYING);
		if (!loading_resource_)
		{
			state_ = ResourceState::FAILED;
			flush_loading_callbacks();
			return;
		}
		if (!loading_resource_->on_ready())
		{
			state_ = ResourceState::FAILED;
			flush_loading_callbacks();
			return;
		}
		resource_ = std::move(loading_resource_);
		state_ = ResourceState::READY;
		flush_loading_callbacks();
	}

	void ResourceEntry::on_dependency_loading_done(ResourceEntry& dependency)
	{
		CP_ASSERT(state_ == ResourceState::WAITING_DEPENDENCIES);
		if (--waiting_dependencies_count_ == 0)
		{
			on_all_dependencies_loading_done();
		}
	}

	void ResourceEntry::on_all_dependencies_loading_done()
	{
		CP_ASSERT(state_ == ResourceState::WAITING_DEPENDENCIES);
		
		// Check if we still have the loading resource
		if (!loading_resource_)
		{
			state_ = ResourceState::FAILED;
			flush_loading_callbacks();
			return;
		}

		// Check if all dependencies are ready
		for (const auto& dependency : loading_resource_->get_dependencies())
		{
			if (!dependency->ready())
			{
				state_ = ResourceState::FAILED;
				flush_loading_callbacks();
				return;
			}
		}

		// Start async readying
		state_ = ResourceState::READYING;
		JobSystem& job_system = JobSystem::get();
		job_system.enqueue([this]()
			{
				this->do_readying();
			});
	}

	void ResourceEntry::flush_loading_callbacks()
	{
		auto callbacks = std::move(loading_done_callbacks_);
		for (auto& func : callbacks)
			func();
	}
}
