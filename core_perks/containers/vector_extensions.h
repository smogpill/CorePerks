// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	template <class T, class Iterator>
	void erase_unordered(std::vector<T>& v, const Iterator& it)
	{
		if (it + 1 != v.end())
			*it = std::move(v.back());
		v.pop_back();
	}

	template <class T>
	void erase_unordered_first(std::vector<T>& v, const T& element)
	{
		auto it = std::find(v.begin(), v.end(), element);
		if (it != v.end())
			erase_unordered(v, it);
	}

	template <class T>
	bool contains(std::vector<T>& v, const T& element)
	{
		return std::find(v.begin(), v.end(), element) != v.end();
	}
}
