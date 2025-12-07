// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/io/resources/base/resource_base.h"
#include "core_perks/io/resources/resource_entry.h"
#include "core_perks/io/resources/resource_manager.h"
#include "core_perks/io/resources/resource_loader.h"
#include "core_perks/io/resources/resource.h"
#include "core_perks/io/streams.h"
#include "core_perks/math/hash.h"
#include "core_perks/threading/job/job_system.h"

namespace cp
{
    ResourceEntry::ResourceEntry(const std::string& id, uint64 id_hash, const cp::Type& type)
        : _id(id)
        , _id_hash(id_hash)
        , _type(&type)
    {
    }

    ResourceEntry::~ResourceEntry()
    {
        delete _resource;
        delete _loading_resource;
    }

    void ResourceEntry::add_loading_dependency()
    {
        ++_nb_loading_dependencies;
    }

    void ResourceEntry::remove_loading_dependency()
    {
        if (--_nb_loading_dependencies == 0)
        {
            _callback_mutex.lock();

            Resource* loaded_resource = _loading_resource.exchange(nullptr);

            // Notify that all dependencies have been loaded
            if (_loading_result)
                _loading_result = loaded_resource->on_dependencies_loaded();

            // Swap the resource and update the state
            if (_loading_result)
            {
                Resource* old_resource = _resource.exchange(loaded_resource);
                delete old_resource;
                _state = ResourceState::READY;
            }
            else
            {
                delete loaded_resource;
                if (_state != ResourceState::READY)
                    _state = ResourceState::FAILED;
            }

            // Call the callbacks
            std::vector<std::function<void(bool)>> callbacks = std::move(_load_callbacks);
            _callback_mutex.unlock();
            for (auto& cb : callbacks)
            {
                cp::JobSystem::get().enqeue([callback = std::move(cb), loading_result = _loading_result]() { callback(loading_result); });
            }

            // Notify the parent resource
            if (_loading_parent)
            {
                if (!_loading_result)
                    _loading_parent->_loading_result = false;
                _loading_parent->remove_loading_dependency();
                _loading_parent = nullptr;
            }
        }
    }

    void ResourceEntry::on_all_refs_removed()
    {
        ResourceManager::get().destroy_entry(*this);
        Base::on_all_refs_removed();
    }

    void ResourceEntry::add_load_callback(Callback callback)
    {
        std::scoped_lock lock(_callback_mutex);
        switch (_state)
        {
        case ResourceState::READY:
        {
            cp::JobSystem::get().enqeue([cb = std::move(callback)]() { cb(true); });
            break;
        }
        case ResourceState::FAILED:
        {
            cp::JobSystem::get().enqeue([cb = std::move(callback)]() { cb(false); });
            break;
        }
        default:
        {
            _load_callbacks.push_back(std::move(callback));
            break;
        }
        }
    }

    void ResourceEntry::load_async(Callback on_done)
    {
        ResourceState expected = ResourceState::NONE;
        if (!_target_state.compare_exchange_strong(expected, ResourceState::READY))
        {

        }
        add_load_callback(on_done);
        
        if (_state.compare_exchange_strong(expected, ResourceState::LOADING))
        {
            add_ref();
            cp::JobSystem::get().enqeue([this]()
                {
                    Resource* new_resource = _type->create<Resource>();
                    Resource* old_resource = _loading_resource.exchange(new_resource);
                    delete old_resource;
                    ResourceLoader loader(*this);
                    add_loading_dependency();
                    _loading_result = _loading_resource.load()->on_load(loader);
                    remove_loading_dependency();
                    remove_ref();
                });
        }
    }

    void ResourceEntry::unload_async()
    {
        ResourceState expected = ResourceState::READY;
        if (_state.compare_exchange_strong(expected, ResourceState::RELEASING))
        {
            add_ref();
            cp::JobSystem::get().enqeue([this]()
                {
                    Resource* resource = _resource.exchange(nullptr);
                    delete resource;
                    remove_ref();
                });
        }
    }

    void ResourceEntry::store_async(Callback on_done)
    {
        add_ref();

        cp::JobSystem::get().enqeue([this, callback = std::move(on_done)]()
            {
                ResourceState expected = ResourceState::READY;
                while (!_state.compare_exchange_weak(expected, ResourceState::SERIALIZING))
                {
                    if (expected != ResourceState::SERIALIZING)
                    {
                        callback(false);
                        return;
                    }
                    expected = ResourceState::READY;
                }
                const Resource* resource = _resource;
                cp::BinaryOutputStream stream;
                resource->on_store(stream);

                expected = ResourceState::SERIALIZING;
                const bool was_serializing = _state.compare_exchange_strong(expected, ResourceState::READY);
                CP_ASSERT(was_serializing);

                namespace fs = std::filesystem;

                const fs::path file_path = get_asset_path();

                remove_ref();

                {
                    auto parent_path = file_path.parent_path();
                    if (!parent_path.empty())
                        fs::create_directories(parent_path);
                }

                std::ofstream file(file_path, std::ios::binary);
                if (!file.is_open())
                {
                    CP_ERROR("Failed to open for write: {}", file_path.string());
                    callback(false);
                    return;
                }

                const std::string content = stream.str();

                file.write(content.data(), content.size());

                if (!file.good())
                {
                    CP_ERROR("Failed to write: {}", file_path.string());
                    callback(false);
                    return;
                }

                file.close();

                callback(true);
            });
    }

    bool ResourceEntry::path_exists() const
    {
        return std::filesystem::exists(get_asset_path());
    }

    auto ResourceEntry::get_name() const -> std::string
    {
        const size_t pos = _id.find_last_of('/');
        return (pos == std::string::npos) ? _id : _id.substr(pos + 1);
    }

    void ResourceEntry::set(Resource* resource)
    {
        Resource* old_resource = _resource.exchange(resource);
        delete old_resource;
    }

    auto ResourceEntry::get_asset_path() const -> std::string
    {
        return (std::filesystem::path(ResourceManager::get().get_assets_path()) / get_id()).string();
    }
}