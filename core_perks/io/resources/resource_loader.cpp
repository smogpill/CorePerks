// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/io/resources/resource_loader.h"
#include "core_perks/io/resources/resource_entry.h"
#include "core_perks/io/resources/resource_manager.h"

namespace cp
{
    ResourceLoader::ResourceLoader(ResourceEntry& entry)
        : _entry(entry)
    {
    }

    auto ResourceLoader::get_asset_path() const -> std::string
    {
        return _entry.get_asset_path();
    }

    bool ResourceLoader::read_as_binary_file(std::vector<uint8>& content)
    {
        namespace fs = std::filesystem;
        const std::filesystem::path path = get_asset_path();

        CP_TRY(fs::exists(path), "File does not exist: {}", path.string());
        CP_TRY(fs::is_regular_file(path), "Path is not a regular file: {}", path.string());

        // Open file in binary mode
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        CP_TRY(file.is_open(), "Failed to open file : {}", path.string());

        // Get file size
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        // Read entire file
        content.clear();
        content.resize(size);
        CP_TRY(file.read(reinterpret_cast<char*>(content.data()), size), "Failed to read file: {}", path.string());

        return true;
    }

    bool ResourceLoader::path_exists() const
    {
        return _entry.path_exists();
    }

    auto ResourceLoader::get_id() const -> const std::string&
    {
        return _entry.get_id();
    }
}
