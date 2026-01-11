// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/patterns/hashed_string.h"
#include "core_perks/math/numerical/hash.h"
#include "core_perks/io/streams/all.h"

namespace cp
{
	// TODO: Shared internal strings to reduce memory usage

	HashedString::HashedString(const std::string& str)
		: hash_(hash::strong::hash64(str))
		, str_(str)
	{
	}

	HashedString::HashedString(std::string&& str)
		: hash_(hash::strong::hash64(str))
		, str_(std::move(str))
	{
	}

	HashedString::HashedString(const char* str)
		: HashedString(std::move(std::string(str)))
	{
	}

	void HashedString::clear()
	{
		hash_ = 0;
		str_.clear();
	}

	const HashedString& HashedString::get_empty()
	{
		static HashedString empty_id;
		return empty_id;
	}

	BinaryInputStream& operator>>(BinaryInputStream& stream, HashedString& id)
	{
		std::string str;
		stream >> str;
		id = std::move(HashedString(std::move(str)));
		return stream;
	}

	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const HashedString& id)
	{
		stream << id.string();
		return stream;
	}
}
