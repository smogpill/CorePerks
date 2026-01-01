// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/resource_entry.h"
#include "core_perks/io/resources/resource_manager.h"

#if 0
namespace cp
{
	template <class T> class TypedResourceHandle;

	class ResourceLoader
	{
	public:
		struct DependencyOptions
		{
			bool _optional = false;
		};

		ResourceLoader(ResourceEntry& entry);
		template <class T>
		TypedResourceHandle<T> add_dependency(const HashedString& id, const DependencyOptions& options = DependencyOptions());
		const HashedString& get_id() const;
		std::string get_resource_path() const;
		bool read_as_binary_file(std::vector<uint8>& content);
		bool path_exists() const;

	private:
		ResourceEntry& _entry;
	};

	template <class T>
	TypedResourceHandle<T> ResourceLoader::add_dependency(const HashedString& id, const DependencyOptions& options)
	{
		TypedResourceHandle<T> handle(id);
		if (handle.entry_->path_exists() || !options._optional)
		{
			//handle.entry_->_loading_parent = &_entry;
			_entry.add_loading_dependency();
			handle.load_async();
		}
		return handle;
	}
}
#endif