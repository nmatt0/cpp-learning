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

void thread_pool::run()
{
    while(!should_shutdown)
    {
        work_element job = NULL;
        {
            std::unique_lock<std::mutex> lk(mutex);
            while (work_queue.empty())
            {
                cond.wait(lk);
                if (should_shutdown)
                {
                    return;
                }
            }
            job = work_queue.front();
            work_queue.pop();
            cond.notify_all();
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
    should_shutdown = true;
    cond.notify_all();
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
    pool_closed = true;
    std::unique_lock<std::mutex> lk(mutex);
    while (!work_queue.empty())
    {
        cond.wait(lk);
    }
    should_shutdown = true;
    
}

void thread_pool::queue_work(work_element work)
{
    if (pool_closed)
    {
        return;
    }
    std::unique_lock<std::mutex> lk(mutex);
    work_queue.push(work);
    cond.notify_one();
}
