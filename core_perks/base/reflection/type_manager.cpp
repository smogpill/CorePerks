// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/base/reflection/type_manager.h"

namespace cp
{
	TypeManager::TypeManager()
	{
		for (Type* type : Type::_types)
			type->init();
	}
}
