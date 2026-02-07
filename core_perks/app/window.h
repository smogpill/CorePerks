// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/math/algebra/vec2.h"

namespace cp
{
	class Window
	{
	public:
		Window(const char* name);
		~Window();

		bool is_minimized() const;
		Vec2u get_client_size() const;
		Vec2u get_active_client_size() const;

#ifdef CP_SDL
		SDL_Window* get_sdl() const { return sdl_window_; }
#endif

	private:
#ifdef CP_SDL
		SDL_Window* sdl_window_ = nullptr;
#endif
	};
}
