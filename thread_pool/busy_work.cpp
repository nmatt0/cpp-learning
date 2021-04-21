#include <chrono>
#include <iostream>
#include "thread_pool.h"

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
    tp.drain();
    tp.stop();
    tp.wait();
}
