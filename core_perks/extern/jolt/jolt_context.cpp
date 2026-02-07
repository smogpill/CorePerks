// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2026 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/extern/jolt/jolt_context.h"

#ifdef CP_JOLT
namespace cp::jolt
{
	static void trace_impl(const char* inFMT, ...)
	{
		va_list list;
		va_start(list, inFMT);
		char buffer[1024];
		vsnprintf(buffer, sizeof(buffer), inFMT, list);
		va_end(list);
		CP_INFO("{}", buffer);
	}

	static bool assert_failed_impl(const char* expression, const char* message, const char* file, uint line)
	{
		cp::message(cp::MessageType::ASSERT, file, line, std::format("({}) {}", expression, message ? message : ""));
		return true;
	};

	Context::Context()
	{
		JPH::RegisterDefaultAllocator();
		JPH::Trace = trace_impl;
		JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = assert_failed_impl);
		JPH::Factory::sInstance = new JPH::Factory();
		JPH::RegisterTypes();
		temp_allocator_ = new JPH::TempAllocatorImpl(10 * 1024 * 1024);
		job_system_ = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);
	}

	Context::~Context()
	{
		delete job_system_;
		delete temp_allocator_;
		JPH::UnregisterTypes();
		delete JPH::Factory::sInstance;
		JPH::Factory::sInstance = nullptr;
	}
}
#endif
