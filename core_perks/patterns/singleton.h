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
			CP_ASSERT(_singleton == nullptr);
			_singleton = static_cast<T*>(this);
		}
		~Singleton()
		{
			_singleton = nullptr;
		}

		static auto get() -> T& { CP_ASSERT(_singleton); return *_singleton; }
		static auto get_as_ptr() -> T* { return _singleton; }
		static bool exists() { return _singleton != nullptr; }

		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;

	private:
		static inline T* _singleton = nullptr;
	};
}
