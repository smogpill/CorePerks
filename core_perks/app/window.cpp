// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/app/window.h"

namespace cp
{
	Window::Window(const char* name)
	{
#ifdef CP_SDL
		SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE;
#ifdef CP_VULKAN
		flags |= SDL_WINDOW_VULKAN;
#endif
		sdl_window_ = SDL_CreateWindow(name, 1024, 768, flags);
		if (!sdl_window_)
			CP_ERROR("Window could not be created! SDL_Error: {}", SDL_GetError());
#endif
	}

	Window::~Window()
	{
#ifdef CP_SDL
		if (sdl_window_)
			SDL_DestroyWindow(sdl_window_);
#endif
	}

	bool Window::is_minimized() const
	{
#ifdef CP_SDL
		return (SDL_GetWindowFlags(sdl_window_) & SDL_WINDOW_MINIMIZED) != 0;
#endif
	}

	Vec2u Window::get_client_size() const
	{
		Vec2u size;
#ifdef CP_SDL
		int w = 0, h = 0;
		SDL_GetWindowSize(sdl_window_, &w, &h);
		size.x_ = (uint32)w;
		size.y_ = (uint32)h;
#endif
		return size;
	}

	Vec2u Window::get_active_client_size() const
	{
		return is_minimized() ? Vec2u(0) : get_client_size();
	}
}
