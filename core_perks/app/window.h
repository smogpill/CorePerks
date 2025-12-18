// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	class Window
	{
	public:
		Window(const std::string& name, uint width = 1024, uint height = 768);
		~Window();
	};
}
