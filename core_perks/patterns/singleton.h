// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	template <class T>
	class Singleton
	{
	public:
		Singleton()
		{
			CP_ASSERT(s_instance == nullptr);
			s_instance = static_cast<T*>(this);
		}
		~Singleton()
		{
			s_instance = nullptr;
		}

		static auto get() -> T& { CP_ASSERT(s_instance); return *s_instance; }
		static auto get_as_ptr() -> T* { return s_instance; }

	private:
		static inline T* s_instance = nullptr;
	};
}
