#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>

class thread_pool
{
public:
    typedef std::function<void()> work_element;

    thread_pool(int size);

    void run();
    void start();
    void stop();
    void wait();
    // let existing work finish but don't accept new work
    void drain();
    void queue_work(work_element work);

private:
    int m_size;
    std::queue<work_element> work_queue;
    std::vector<std::thread*> thread_list;
    bool should_shutdown = false;
    bool pool_closed = false;
    std::mutex mutex;
    std::condition_variable cond;
};
