// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "app.h"

namespace cp
{
	App::App()
	{
		/*
		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			CP_ERROR("SDL could not initialize! SDL_Error: {}", SDL_GetError());
			return 1;
		}
		sdl_initialized_ = true;
		*/
	}

	App::~App()
	{
		/*
		if (sdl_initialized_)
		{
			SDL_Quit();
		}
		*/
	}
}
