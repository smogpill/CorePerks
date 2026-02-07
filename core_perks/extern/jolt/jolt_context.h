// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2026 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

#ifdef CP_JOLT
namespace JPH
{
	class TempAllocator;
	class JobSystem;
}
namespace cp::jolt
{
	class Context : public Singleton<Context>
	{
	public:
		Context();
		~Context();

		JPH::TempAllocator* get_temp_allocator() const { return temp_allocator_; }
		JPH::JobSystem* get_job_system() const { return job_system_; }

	private:
		JPH::TempAllocator* temp_allocator_ = nullptr;
		JPH::JobSystem* job_system_ = nullptr;
	};
}
#endif
