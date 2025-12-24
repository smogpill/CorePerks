// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/patterns/string_id.h"
#include "core_perks/math/numerical/hash.h"
#include "core_perks/io/streams/all.h"

namespace cp
{
	// TODO: Shared internal strings to reduce memory usage

	StringID::StringID(const std::string& str)
		: hash_(hash::strong::hash64(str))
		, str_(str)
	{
	}

	StringID::StringID(std::string&& str)
		: hash_(hash::strong::hash64(str))
		, str_(std::move(str))
	{
	}

	void StringID::clear()
	{
		hash_ = 0;
		str_.clear();
	}

	BinaryInputStream& operator>>(BinaryInputStream& stream, StringID& id)
	{
		std::string str;
		stream >> str;
		id = std::move(StringID(std::move(str)));
		return stream;
	}

	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const StringID& id)
	{
		stream << id.str();
		return stream;
	}
}
