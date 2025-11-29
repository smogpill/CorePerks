// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/resource_holder.h"
#include "core_perks/io/resources/resource_manager.h"

namespace cp
{
	template <class T> class ResourceHandle;

	class ResourceLoader
	{
	public:
		struct DependencyOptions
		{
			bool _optional = false;
		};

		ResourceLoader(ResourceHolder& holder);
		template <class T>
		auto add_dependency(const std::string& id, const DependencyOptions& options = DependencyOptions()) -> ResourceHandle<T>;
		auto get_id() const -> const std::string&;
		auto get_asset_path() const -> std::string;
		bool read_as_binary_file(std::vector<uint8>& content);
		bool path_exists() const;

	private:
		ResourceHolder& _holder;
	};

	template <class T>
	auto ResourceLoader::add_dependency(const std::string& id, const DependencyOptions& options) -> ResourceHandle<T>
	{
		ResourceHandle<T> handle(id);
		if (handle._holder->path_exists() || !options._optional)
		{
			handle._holder->_loading_parent = &_holder;
			_holder.add_loading_dependency();
			handle.load_async();
		}
		return handle;
	}
}
