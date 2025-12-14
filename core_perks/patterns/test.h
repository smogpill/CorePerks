// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/base/reflection/type.h"

namespace cp
{
	class Test
	{
		CP_CLASS(Test);
	public:
		static void execute_all();

		virtual ~Test() = default;
		virtual void execute() = 0;
	};
}

#define _CP_UNIQUE_TEST_NAME(_test_case_name_, _test_name_) _test__##_test_case_name_##__##_test_name_

#define _CP_TEST_IMPL(_test_case_name_, _test_name_, _parent_class_) \
	class _CP_UNIQUE_TEST_NAME(_test_case_name_, _test_name_) : public _parent_class_ \
	{ \
		CP_BASE(_parent_class_); \
		CP_CLASS(TestImpl); \
	public: \
		virtual void execute() override; \
	}; \
	CP_DEFINE_CLASS(_CP_UNIQUE_TEST_NAME(_test_case_name_, _test_name_)) {} \
	void _CP_UNIQUE_TEST_NAME(_test_case_name_, _test_name_)::execute()

#define CP_TEST(_test_case_name_, _test_name_) _CP_TEST_IMPL(_test_case_name_, _test_name_, Test)
#define CP_FIXTURE_TEST(_fixture_class_name_, _test_name_) _CP_TEST_IMPL(_fixture_class_name_, _test_name_, _fixture_class_name_)

#define CP_EXPECT(_cond_) CP_SAFE_SCOPE( if(!(_cond_) && cp::return_true()){ CP_BREAKPOINT(); } )
