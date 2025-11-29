// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/io/resources/resource_holder.h"
#include "core_perks/io/resources/resource_manager.h"
#include "core_perks/io/resources/resource_loader.h"
#include "core_perks/io/resources/resource.h"
#include "core_perks/io/streams.h"
#include "core_perks/math/hash.h"
#include "core_perks/threading/job/job_system.h"

namespace cp
{
    ResourceHolder::ResourceHolder(const std::string& id)
        : _id(id)
        , _id_hash(hash::strong::hash64(id))
    {
    }

    ResourceHolder::~ResourceHolder()
    {
        delete _resource;
        delete _loading_resource;
    }

    void ResourceHolder::add_loading_dependency()
    {
        ++_nb_loading_dependencies;
    }

    void ResourceHolder::remove_loading_dependency()
    {
        if (--_nb_loading_dependencies == 0)
        {
            _callback_mutex.lock();

            Resource* loadedResource = _loading_resource.exchange(nullptr);

            // Notify that all dependencies have been loaded
            if (_loading_result)
                _loading_result = loadedResource->on_dependencies_loaded();

            // Swap the resource and update the state
            if (_loading_result)
            {
                Resource* oldResource = _resource.exchange(loadedResource);
                delete oldResource;
                _state = ResourceState::READY;
            }
            else
            {
                delete loadedResource;
                if (_state != ResourceState::READY)
                    _state = ResourceState::FAILED;
            }

            // Call the callbacks
            std::vector<std::function<void(bool)>> callbacks = std::move(_load_callbacks);
            _callback_mutex.unlock();
            for (auto& cb : callbacks)
            {
                cp::JobSystem::get().enqeue([callback = std::move(cb), loadingResult = _loading_result]() { callback(loadingResult); });
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

    void ResourceHolder::on_all_refs_removed()
    {
        ResourceManager::get().destroy_holder(*this);
        Base::on_all_refs_removed();
    }

    void ResourceHolder::add_load_callback(Callback callback)
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

    void ResourceHolder::load_async(std::function<Resource* ()> createFunc)
    {
        ResourceState expected = ResourceState::NONE;
        if (_state.compare_exchange_strong(expected, ResourceState::LOADING))
        {
            add_ref();
            cp::JobSystem::get().enqeue([this, create = std::move(createFunc)]()
                {
                    Resource* oldResource = _loading_resource.exchange(create());
                    delete oldResource;
                    ResourceLoader loader(*this);
                    add_loading_dependency();
                    _loading_result = _loading_resource.load()->on_load(loader);
                    remove_loading_dependency();
                    remove_ref();
                });
        }
    }

    void ResourceHolder::unload_async()
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

    void ResourceHolder::store_async(Callback callbackFunc)
    {
        add_ref();

        cp::JobSystem::get().enqeue([this, callback = std::move(callbackFunc)]()
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
                const bool wasSerializing = _state.compare_exchange_strong(expected, ResourceState::READY);
                CP_ASSERT(wasSerializing);

                namespace fs = std::filesystem;

                const fs::path filePath = get_asset_path();

                remove_ref();

                {
                    auto parentPath = filePath.parent_path();
                    if (!parentPath.empty())
                        fs::create_directories(parentPath);
                }

                std::ofstream file(filePath, std::ios::binary);
                if (!file.is_open())
                {
                    CP_ERROR("Failed to open for write: {}", filePath.string());
                    callback(false);
                    return;
                }

                const std::string content = stream.str();

                file.write(content.data(), content.size());

                if (!file.good())
                {
                    CP_ERROR("Failed to write: {}", filePath.string());
                    callback(false);
                    return;
                }

                file.close();

                callback(true);
            });
    }

    bool ResourceHolder::path_exists() const
    {
        return std::filesystem::exists(get_asset_path());
    }

    auto ResourceHolder::get_name() const -> std::string
    {
        const size_t pos = _id.find_last_of('/');
        return (pos == std::string::npos) ? _id : _id.substr(pos + 1);
    }

    void ResourceHolder::set(Resource* resource)
    {
        Resource* oldResource = _resource.exchange(resource);
        delete oldResource;
    }

    auto ResourceHolder::get_asset_path() const -> std::string
    {
        return (std::filesystem::path(ResourceManager::get().get_assets_path()) / get_id()).string();
    }
}