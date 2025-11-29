// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/threading/job/job_system.h"

namespace cp
{
    JobSystem::JobSystem(uint numThreads)
    {
        // Create worker threads
        _threads.reserve(numThreads);
        for (uint i = 0; i < numThreads; ++i)
        {
            _threads.emplace_back(&JobSystem::WorkerThread, this);
        }

        // Create blocking I/O thread
        _blocking_thread = std::jthread([this](std::stop_token st)
            {
                while (!st.stop_requested())
                {
                    Job job;
                    {
                        std::unique_lock lock(_blocking_mutex);
                        _blocking_condition.wait(lock, [this, &st]
                            {
                                return !_blocking_queue.empty() || st.stop_requested();
                            });
                        if (st.stop_requested()) break;
                        job = std::move(_blocking_queue.front());
                        _blocking_queue.pop_front();
                    }
                    job();
                }
            });
    }

    JobSystem::~JobSystem()
    {
        stop();
    }

    void JobSystem::WorkerThread()
    {
        while (_running)
        {
            Job job;
            {
                std::unique_lock lock(_queue_mutex);
                _condition.wait(lock, [this]
                    {
                        return !_job_queue.empty() || !_running;
                    });

                if (!_running && _job_queue.empty())
                    break;

                job = std::move(_job_queue.front());
                _job_queue.pop_front();
            }
            job();
        }
    }

    void JobSystem::stop()
    {
        if (!_running.exchange(false))
            return;

        _condition.notify_all();
        _blocking_condition.notify_all();

        for (auto& thread : _threads)
        {
            if (thread.joinable())
                thread.join();
        }

        _blocking_thread.request_stop();
        _blocking_condition.notify_all();
        if (_blocking_thread.joinable())
        {
            _blocking_thread.join();
        }
    }
}