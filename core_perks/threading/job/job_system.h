// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/threading/job/job.h"
#include "core_perks/patterns/singleton.h"

namespace cp
{
    class JobSystem : public Singleton<JobSystem>
    {
    public:
        JobSystem(uint numThreads = std::thread::hardware_concurrency());
        ~JobSystem();

        /// Enqueue a regular function/lambda as a job
        template<typename Callable>
        void enqeue(Callable&& callable);

        /// Helper function to enqueue blocking jobs
        template<typename Callable>
        void enqueue_blocking(Callable&& callable);

        /// Enqueue a coroutine and return a handle to await its result
        template<typename CoroutineJob>
        auto enqueue_coroutine(CoroutineJob&& coroTask);

        /// Run a blocking function on a dedicated thread (for I/O)
        template<typename Callable>
        auto run_blocking_job(Callable&& callable);

        /// For the resource manager's dependency waiting
        void resume_coroutine(std::coroutine_handle<> h) { enqeue([h]() { h.resume(); }); }
        void stop();
        bool is_running() const { return _running; }

    private:
        void WorkerThread();

        std::vector<std::jthread> _threads;
        std::deque<Job> _job_queue;
        std::mutex _queue_mutex;
        std::condition_variable _condition;
        std::atomic<bool> _running = true;

        // Separate queue and thread for blocking operations (like file I/O)
        std::deque<Job> _blocking_queue;
        std::mutex _blocking_mutex;
        std::condition_variable _blocking_condition;
        std::jthread _blocking_thread;
    };

    template<typename Callable>
    void JobSystem::enqeue(Callable&& callable)
    {
        {
            std::unique_lock lock(_queue_mutex);
            _job_queue.emplace_back(std::forward<Callable>(callable));
        }
        _condition.notify_one();
    }

    template<typename Callable>
    void JobSystem::enqueue_blocking(Callable&& callable)
    {
        {
            std::unique_lock lock(_blocking_mutex);
            _blocking_queue.emplace_back(std::forward<Callable>(callable));
        }
        _blocking_condition.notify_one();
    }

    template<typename CoroutineJob>
    auto JobSystem::enqueue_coroutine(CoroutineJob&& coro_task)
    {
        using ResultType = decltype(coro_task.get_result());
        struct Awaiter
        {
            CoroutineJob _task;

            bool await_ready() const { return false; }
            void await_suspend(std::coroutine_handle<> h)
            {
                // Wrap the coroutine task and continuation in a job
                JobSystem::s_instance->enqeue([this, h]() mutable
                    {
                        _task.start(); // Execute the coroutine until it suspends or completes
                        if (_task.done())
                        {
                            // If done immediately, resume the caller directly
                            h.resume();
                        }
                        else {
                            // Otherwise, store the handle for later resumption
                            _task.set_continuation(h);
                        }
                    });
            }
            ResultType await_resume() { return _task.get_result(); }
        };
        return Awaiter{ this, std::forward<CoroutineJob>(coro_task) };
    }

    template<typename Callable>
    auto JobSystem::run_blocking_job(Callable&& callable)
    {
        using ResultType = decltype(callable());
        struct BlockingAwaiter
        {
            Callable callable;

            bool await_ready() const { return false; }
            void await_suspend(std::coroutine_handle<> h)
            {
                JobSystem::get().enqueue_blocking([this, h]()
                    {
                        _result = callable();
                        JobSystem::get().enqeue([h]() { h.resume(); });
                    });
            }
            ResultType await_resume() { return std::move(_result); }

            std::optional<ResultType> _result;
        };
        return BlockingAwaiter{ this, std::forward<Callable>(callable) };
    }

    /// Basic awaitable for resuming on the job system threads
    struct ScheduleOnJobSystem
    {
        bool await_ready() const { return false; }
        void await_suspend(std::coroutine_handle<> h) { JobSystem::get().enqeue([h]() { h.resume(); }); }
        void await_resume() const {}
    };
}
