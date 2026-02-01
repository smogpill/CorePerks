// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

/// Usage:
/// coSCOPE_EXIT { instructions };

namespace cp::detail
{
	struct scope_exit
	{
		template <class F>
		scope_exit(F f) : func_(f) {}
		~scope_exit() { func_(); }
		std::function<void()> func_;
	};
}

#define CP_SCOPE_EXIT cp::detail::scope_exit CP_CONCAT(_scope_exit_, __COUNTER__) = [&]()
