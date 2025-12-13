// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

class Job
{
public:
    Job() = default;

    template<typename Callable>
    Job(Callable&& callable)
        : function_([callable = std::forward<Callable>(callable)]() { callable(); })
    {
    }

    void operator()() { function_(); }

private:
    std::function<void()> function_;
};
