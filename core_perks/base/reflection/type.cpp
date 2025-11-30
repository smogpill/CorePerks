// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/base/reflection/type.h"
#include "core_perks/base/reflection/type_manager.h"

namespace cp
{
	Type::Type(const char* name)
		: _name(name)
	{
		_types.push_back(this);
	}

	void Type::init(Type* type)
	{
		if (type == nullptr)
		{
			if (_initialized)
				return;
			_initialized = true;
			type = this;
		}
		if (_base)
			_base->init(type);
		if (_init_type_func)
			_init_type_func(*type);
	}

	namespace detail
	{
		std::vector<detail::TypeAutoRegistrator*> registrators;

		void TypeAutoRegistrator::add_registrator(TypeAutoRegistrator& registrator)
		{
			registrators.push_back(&registrator);
		}

		TypeAutoRegistrator* TypeAutoRegistrator::get_registrator(int idx)
		{
			return registrators[idx];
		}

		int TypeAutoRegistrator::get_registrator_count()
		{
			return (int)registrators.size();
		}
	}
}
