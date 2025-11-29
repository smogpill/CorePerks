// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

#ifdef CP_VULKAN
namespace cp
{
	std::string get_vulkan_error(VkResult result);
}

#define CP_VULKAN_TRY(_cond_, ...) CP_SAFE_SCOPE( VkResult _result_ = _cond_; CP_TRY(_result_ == VK_SUCCESS, "{}", cp::get_vulkan_error(_result_).c_str()); )
#define CP_VULKAN_CHECK(_cond_, ...) CP_SAFE_SCOPE( VkResult _result_ = _cond_; CP_CHECK(_result_ == VK_SUCCESS,"{}", cp::get_vulkan_error(_result_).c_str()); )

#endif
