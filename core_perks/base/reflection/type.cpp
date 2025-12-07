// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/base/reflection/type.h"
#include "core_perks/base/reflection/type_manager.h"
#include "core_perks/math/hash.h"

namespace cp
{
	Type::Type(const char* name)
		: _name(name)
		, _name_hash(hash::fast::hash32(_name))
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
		if (_init)
			_init(*type);
	}
}
