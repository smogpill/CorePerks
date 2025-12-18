// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	class App
	{
	public:
		App();
		~App();

	private:
		bool sdl_initialized_ = false;
	};
}
