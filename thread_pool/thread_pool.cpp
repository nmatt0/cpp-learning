#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <cassert>
#include <functional>
#include <iostream>
#include "thread_pool.h"

thread_pool::thread_pool(int size) : m_size(size)
{
}

thread_pool::~thread_pool()
{
    for (std::thread * t : thread_list)
    {
        delete(t);
    }
}

void thread_pool::run()
{
    while(!should_shutdown)
    {
        work_element job = NULL;
        {
            std::unique_lock<std::mutex> lk(mutex);
            while (work_queue.empty())
            {
                cond_queue.wait(lk);
                if (should_shutdown)
                {
                    return;
                }
            }
            job = work_queue.front();
            work_queue.pop();
            cond_drain.notify_one();
        }
        job();
    }
}

void thread_pool::start()
{
    for (int i = 0; i < m_size; i++)
    {
        std::thread* t = new std::thread(&thread_pool::run,this);
        assert(t);
        thread_list.push_back(t);
    }
}

void thread_pool::stop()
{
    std::unique_lock<std::mutex> lk(mutex);
    should_shutdown = true;
    cond_queue.notify_all();
}

void thread_pool::wait()
{
    for (std::thread * t : thread_list)
    {
        t->join();
    }
}

// let existing work finish but don't accept new work
void thread_pool::drain()
{
    std::unique_lock<std::mutex> lk(mutex);
    pool_closed = true;
    while (!work_queue.empty())
    {
        cond_drain.wait(lk);
    }
}

void thread_pool::queue_work(work_element work)
{
    std::unique_lock<std::mutex> lk(mutex);
    if (pool_closed)
    {
        return;
    }
    work_queue.push(work);
    cond_queue.notify_one();
}
