// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include <cstdlib>

#ifdef CP_XXHASH
#include <xxHash/xxhash.h>
#endif

#ifdef CP_RAPIDJSON
#include <rapidjson/rapidjson.h>
#endif

#ifdef CP_IMGUI
#ifdef CP_VULKAN
#include <imgui/backends/imgui_impl_vulkan.h>
#endif
#ifdef CP_SDL
#include <imgui/backends/imgui_impl_sdl3.h>
#endif
#endif
