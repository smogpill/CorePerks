// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

#if CP_DEBUG
#define _ITERATOR_DEBUG_LEVEL 2
#endif
#include <cassert>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <span>
#include <vector>
#include <deque>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <format>
#include <utility>
#include <filesystem>
#include <memory>
#include <future>
#include <thread>
#include <coroutine>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <nmmintrin.h>
#include <immintrin.h>
#include <bit>
#include <initializer_list>

#ifdef CP_XXHASHCT
#include <xxhashct/xxh32.hpp>
#include <xxhashct/xxh64.hpp>
#endif

#ifdef CP_GLM
#define GLM_FORCE_XYZW_ONLY
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/common.hpp>
#include <glm/matrix.hpp> 
#include <glm/trigonometric.hpp>
#include <glm/vector_relational.hpp>
#include <glm/ext/matrix_transform.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/ext/quaternion_common.hpp>
#include <glm/ext/vector_double3.hpp>
#include <glm/gtc/quaternion.hpp>
#endif

#ifdef CP_VULKAN
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#endif

#ifdef CP_VMA
#include <vk_mem_alloc.h>
#endif

#define VC_EXTRALEAN
#define NOLANGUAGE
#define NOKEYBOARDINFO
#define NOLSTRING
#define NOGDI
#define NOMINMAX
#include <Windows.h>

#ifdef CP_SDL
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_keyboard.h>
#endif

#ifdef CP_TINYOBJLOADER
#include <tinyobjloader/tiny_obj_loader.h>
#endif
