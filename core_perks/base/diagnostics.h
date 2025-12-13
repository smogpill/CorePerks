// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	enum class MessageType
	{
		INFO,
		ERROR,
		ASSERT,
		FATAL
	};
	void message(MessageType type, const char* file, uint line, const std::string& message);
	[[noreturn]] void crash();
}

#define CP_ASSERT(_cond_, ...) CP_SAFE_SCOPE(if (!(_cond_)) [[unlikely]] { cp::message(cp::MessageType::ASSERT, __FILE__, __LINE__, #_cond_); CP_BREAKPOINT(); } )
#define CP_INFO(...) cp::message(cp::MessageType::INFO, __FILE__, __LINE__, std::format(__VA_ARGS__))
#define CP_ERROR(...) cp::message(cp::MessageType::ERROR, __FILE__, __LINE__, std::format(__VA_ARGS__))
#define CP_FATAL(...) CP_SAFE_SCOPE( cp::message(cp::MessageType::FATAL, __FILE__, __LINE__, std::format(__VA_ARGS__)); cp::crash(); )
#define CP_TRY(_cond_, ...) CP_SAFE_SCOPE( if(!(_cond_)) [[unlikely]] { __VA_OPT__(CP_ERROR(__VA_ARGS__)); CP_BREAKPOINT(); return false; } )
#define CP_TRY_NO_RETURN_VALUE(_cond_, ...) CP_SAFE_SCOPE( if(!(_cond_)) [[unlikely]] { __VA_OPT__(CP_ERROR(__VA_ARGS__)); CP_BREAKPOINT(); return; } )
#define CP_CHECK(_cond_, ...) CP_SAFE_SCOPE( if(!(_cond_)) [[unlikely]] { __VA_OPT__(CP_ERROR(__VA_ARGS__)); CP_BREAKPOINT(); } )
