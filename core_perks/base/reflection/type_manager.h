// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/singleton.h"

namespace cp
{
	class TypeManager : public Singleton<TypeManager>
	{
	public:
		TypeManager();
	};
}
