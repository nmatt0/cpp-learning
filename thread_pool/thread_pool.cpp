#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <vector>
#include <cassert>
#include <functional>
#include <iostream>

class thread_pool
{
public:
    typedef std::function<void()> work_element;

    thread_pool(int size) : m_size(size)
    {
    }

    void run()
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

    void start()
    {
        for (int i = 0; i < m_size; i++)
        {
            std::thread* t = new std::thread(&thread_pool::run,this);
            assert(t);
            thread_list.push_back(t);
        }
    }

    void stop()
    {
        should_shutdown = true;
        cond.notify_all();
    }

    void wait()
    {
        for (std::thread * t : thread_list)
        {
            t->join();
        }
    }

    // let existing work finish but don't accept new work
    void drain()
    {
        pool_closed = true;
        std::unique_lock<std::mutex> lk(mutex);
        while (!work_queue.empty())
        {
            cond.wait(lk);
        }
        should_shutdown = true;
        
    }

    void queue_work(work_element work)
    {
        if (pool_closed)
        {
            return;
        }
        std::unique_lock<std::mutex> lk(mutex);
        work_queue.push(work);
        cond.notify_one();
    }

private:
    int m_size;
    std::queue<work_element> work_queue;
    std::vector<std::thread*> thread_list;
    bool should_shutdown = false;
    bool pool_closed = false;
    std::mutex mutex;
    std::condition_variable cond;
};

void busy_work()
{
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "busy work complete" << std::endl << std::flush;
}

int main()
{
    thread_pool tp(5);
    tp.start();
    for (int i = 0; i < 10; i++)
    {
        tp.queue_work([]() {
            busy_work();
        });
    }
    tp.drain(); // alternative to tp.stop()
    tp.wait();
}
