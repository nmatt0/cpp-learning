#include <iostream>
#include <sstream>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <thread>
#include <vector>

static volatile bool done = false;
static std::mutex mutex;
static std::condition_variable cond1;
static std::condition_variable cond2;
static std::queue<int> q;
static const int MAX_Q_SIZE = 10;

bool consumer_job(int & val)
{
    std::unique_lock<std::mutex> lk(mutex);
    while(q.empty() && !done)
    {
        std::cv_status status = cond1.wait_for(lk, std::chrono::microseconds(500));
        if (status == std::cv_status::timeout)
        {
            return false;
        }
    }
    if (done)
    {
        return false;
    }
    val = q.front();
    q.pop();
    cond2.notify_one();
    return true;
}

void producer_job(int val)
{
    std::unique_lock<std::mutex> lk(mutex);
    while (q.size() >= MAX_Q_SIZE)
    {
        cond2.wait(lk);
    }
    q.push(val);
    cond1.notify_one();
}

void consumer(int cid)
{
    int val;
    while(!done)
    {
        if(consumer_job(val))
        {
            std::stringstream ss;
            ss << "(" << cid << ") " << val << std::endl;
            std::cout << ss.str() << std::flush;
        }
    }
}

void producer()
{
    for (int i = 0; i < 100; i++)
    {
        producer_job(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main()
{
    //std::vector<std::thread*> consumers;
    std::vector<std::thread*> producers;
    std::vector<std::thread*> consumers;

    std::thread p1(&producer);
    std::thread p2(&producer);
    std::thread c1(&consumer,1);
    std::thread c2(&consumer,2);

    producers.push_back(&p1);
    producers.push_back(&p2);
    consumers.push_back(&c1);
    consumers.push_back(&c2);

    for (std::thread * t : producers)
    {
        t->join();
    }
    done = true;
    for (std::thread * t : consumers)
    {
        t->join();
    }
    return 0;
}
