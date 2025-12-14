// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/patterns/test.h"

namespace cp
{
	CP_DEFINE_CLASS(Test) {}

	void Test::execute_all()
	{
		for (const Type* type : Test::get_type_static().get_derived())
		{
			Test* test = type->create<Test>();
			test->execute();
			delete test;
		}
	}
}
