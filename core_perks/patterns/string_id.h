// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	class BinaryInputStream; class BinaryOutputStream;

	class StringID
	{
	public:
		StringID() = default;
		StringID(const std::string& str);
		StringID(std::string&& str);
		bool operator==(const StringID& other) const { return hash_ == other.hash_; }
		bool operator!=(const StringID& other) const { return hash_ != other.hash_; }
		StringID& operator=(const StringID& other) = default;
		StringID& operator=(StringID&& other) = default;
		uint64 hash() const { return hash_; }
		const std::string& str() const { return str_; }
		void clear();
		bool empty() const { return hash_ == 0; }

	private:
		uint64 hash_ = 0;
		std::string str_;
	};

	BinaryInputStream& operator>>(BinaryInputStream& stream, StringID& id);
	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const StringID& id);
}
