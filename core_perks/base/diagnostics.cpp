// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/base/diagnostics.h"

namespace cp
{
    void Message(MessageType type, const char* file, uint line, const std::string& message)
    {
        const char* typeStr = "<unknown>";
        switch (type)
        {
        case MessageType::INFO: typeStr = "Info"; break;
        case MessageType::ERROR: typeStr = "Error"; break;
        case MessageType::ASSERT: typeStr = "Assert"; break;
        default: break;
        }
        const std::string text = std::format("{}({}): {}: {}\n", file, line, typeStr, message.c_str());
        if (type == MessageType::INFO)
            std::cout << text;
        else
            std::cerr << text;
#ifdef CP_WINDOWS
        ::OutputDebugStringA(text.c_str());
#endif
    }
}
