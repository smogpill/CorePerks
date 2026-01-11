// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	class BinaryInputStream; class BinaryOutputStream;

	class HashedString
	{
	public:
		HashedString() = default;
		HashedString(const HashedString& other) = default;
		HashedString(HashedString&& other) = default;
		HashedString(const char* str);
		HashedString(const std::string& str);
		HashedString(std::string&& str);
		bool operator==(const HashedString& other) const { return hash_ == other.hash_ && str_ == other.str_; }
		bool operator!=(const HashedString& other) const { return !operator==(other); }
		HashedString& operator=(const HashedString& other) = default;
		HashedString& operator=(HashedString&& other) = default;
		uint64 hash() const { return hash_; }
		const std::string& string() const { return str_; }
		void clear();
		bool empty() const { return str_.empty(); }
		static const HashedString& get_empty();

	private:
		uint64 hash_ = 0;
		std::string str_;
	};

	BinaryInputStream& operator>>(BinaryInputStream& stream, HashedString& id);
	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const HashedString& id);
}
