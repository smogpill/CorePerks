// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	class BinaryOutputStream;
	class ResourceLoader;

	class Resource
	{
	public:
		virtual ~Resource() {}

		virtual bool on_load(ResourceLoader& loader) = 0 { return true; }
		virtual bool on_dependencies_loaded() { return true; }
		virtual void on_store(cp::BinaryOutputStream& stream) const {}
	};
}
