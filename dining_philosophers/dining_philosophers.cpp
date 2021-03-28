#include <iostream>
#include <cassert>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <condition_variable>

volatile static bool done = false;
static std::mutex mutex;
static std::condition_variable cond;

class Fork
{
public:
    Fork()
    {
    }

    bool InUse()
    {
        return inUse;
    }

    void takeFork()
    {
        assert(!inUse);
        inUse = true;
    }

    void returnFork()
    {
        assert(inUse);
        inUse = false;
    }

private:
    bool inUse = false;
};

class Philosopher
{
public:
    Philosopher(Fork * lf, Fork * rf)
    {
        leftFork = lf;
        rightFork = rf;
    }

    Fork * leftFork = nullptr;
    Fork * rightFork = nullptr;
    int foodEaten = 0;
};

void EatOrThink(Philosopher * p)
{
    while(!done)
    {
        {
            std::unique_lock<std::mutex> lk(mutex);
            while(p->leftFork->InUse() || p->rightFork->InUse())
            {
                cond.wait(lk); // thinking
            }
            p->leftFork->takeFork();
            p->rightFork->takeFork();
        }
        // eating
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        p->leftFork->returnFork();
        p->rightFork->returnFork();
        cond.notify_all();
        p->foodEaten++;
        // take a break from eating
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

int main()
{
    int runTime = 10; // in seconds
    Fork f1;
    Fork f2;
    Fork f3;
    Fork f4;
    Fork f5;
    Philosopher p1(&f1,&f5);
    Philosopher p2(&f2,&f1);
    Philosopher p3(&f3,&f2);
    Philosopher p4(&f4,&f3);
    Philosopher p5(&f5,&f4);
    std::vector<Philosopher*> philosopher_list;
    philosopher_list.push_back(&p1);
    philosopher_list.push_back(&p2);
    philosopher_list.push_back(&p3);
    philosopher_list.push_back(&p4);
    philosopher_list.push_back(&p5);
    std::vector<std::thread*> thread_list;

    for (Philosopher * p : philosopher_list)
    {
        thread_list.push_back(new std::thread(&EatOrThink,p));
    }
    std::cout << "Philosophers will eat for " << runTime << " seconds." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    done = true;
    for (std::thread * t : thread_list)
    {
        t->join();
    }
    for (Philosopher * p : philosopher_list)
    {
        std::cout << "foodEaten: " << p->foodEaten << std::endl;
    }
    for (std::thread * t : thread_list)
    {
        delete(t);
    }
    return 0;
}



