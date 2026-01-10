// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/resources/resource_entry.h"
#include "core_perks/io/resources/resource_manager.h"
#include "core_perks/io/resources/resource_handle.h"
#include "core_perks/io/streams/binary_serializer.h"
#include "core_perks/threading/job/job_system.h"

namespace cp
{
	ResourceEntry::ResourceEntry(const ResourceID& id)
		: id_(id)
	{
	}

	ResourceEntry::~ResourceEntry()
	{
		manager().on_entry_destroyed(*this);
	}

	void ResourceEntry::set(const RefPtr<Resource>& resource)
	{
		std::scoped_lock lock(mutex_);
		resource_ = resource;
		if (resource_)
			resource_->entry_ = this;
	}

	void ResourceEntry::load_async(std::function<void()>&& on_done)
	{
		std::scoped_lock lock(mutex_);
		if (state_ == ResourceState::READY || state_ == ResourceState::FAILED)
		{
			on_done();
			return;
		}
		loading_done_callbacks_.emplace_back(std::move(on_done));
		ResourceManager::LoadRequest request;
		request.handle_.entry_ = this;
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
		MappedFileRegion mapped_region = manager().map_resource(id_);
		if (!mapped_region.is_mapped())
		{
			state_ = ResourceState::FAILED;
			flush_loading_callbacks();
			return;
		}

		RefPtr<Resource> resource(type_->create<Resource>());

		if (!resource->on_read(mapped_region))
		{
			state_ = ResourceState::FAILED;
			flush_loading_callbacks();
			return;
		}

		BinarySerializer serializer(std::move(mapped_region), BinarySerializer::READ);
		resource->on_serialize(serializer);
		if (serializer.failed())
		{
			state_ = ResourceState::FAILED;
			flush_loading_callbacks();
			return;
		}

		loading_resource_ = std::move(resource);
		const auto& dependencies = resource->get_dependencies();
		if (!dependencies.empty())
		{
			state_ = ResourceState::WAITING_DEPENDENCIES;
			waiting_dependencies_count_ = (uint32)dependencies.size();
			ResourceHandle self(this);
			for (const ResourceHandle& dependency : dependencies)
			{
				auto callback = [=]()
					{
						self->on_dependency_loading_done(*dependency);
					};
				dependency->load_async(callback);
			}
			return;
		}

		state_ = ResourceState::READYING;
		do_readying();
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
		state_ = ResourceState::READY;
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
			if (!dependency->is_ready())
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
