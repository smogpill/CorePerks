// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/extern/imgui/imgui.h"

#ifdef CP_IMGUI
namespace cp
{
	ImGuiUtils::ImGuiUtils()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
	}

	ImGuiUtils::~ImGuiUtils()
	{
#ifdef CP_VULKAN
		ImGui_ImplVulkan_Shutdown();
#endif
#ifdef CP_SDL
		ImGui_ImplSDL3_Shutdown();
#endif
		ImGui::DestroyContext();
	}

	void ImGuiUtils::begin_frame()
	{
#ifdef CP_VULKAN
		ImGui_ImplVulkan_NewFrame();
#endif
#ifdef CP_SDL
		ImGui_ImplSDL3_NewFrame();
#endif
		::ImGui::NewFrame();
	}

	void ImGuiUtils::end_frame()
	{
		::ImGui::Render();
	}
}
#endif