// CorePerks (https://github.com/smogpill/CorePerks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	class BinaryOutputStream : public std::ostringstream
	{
		using Base = std::ostringstream;
	public:
		using Base::operator<<;

		BinaryOutputStream();

		template<typename T>
		auto operator<<(const std::vector<T>& vec) -> BinaryOutputStream&;
	};

	template<typename T>
	BinaryOutputStream& BinaryOutputStream::operator<<(const std::vector<T>& vec)
	{
		size_t size = vec.size();
		write(reinterpret_cast<const char*>(&size), sizeof(size_t));
		if (!vec.empty())
			write(reinterpret_cast<const char*>(vec.data()), vec.size() * sizeof(T));
		return *this;
	}
	/*
	template<typename T>
	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const std::vector<T>& vec)
	{
		size_t size = vec.size();
		stream.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
		if (!vec.empty())
			stream.write(reinterpret_cast<const char*>(vec.data()), vec.size() * sizeof(T));
		return stream;
	}
	*/
}
