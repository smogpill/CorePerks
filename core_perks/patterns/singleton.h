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
			CP_ASSERT(singleton_ == nullptr);
			singleton_ = static_cast<T*>(this);
		}
		~Singleton()
		{
			singleton_ = nullptr;
		}

		static T& get() { CP_ASSERT(singleton_); return *singleton_; }
		static T* get_as_ptr() { return singleton_; }
		static bool exists() { return singleton_ != nullptr; }

		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;

	private:
		static inline T* singleton_ = nullptr;
	};
}
