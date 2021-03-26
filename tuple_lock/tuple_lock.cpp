#include <iostream>
#include <set>
#include <tuple>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>
#include <sstream>

static std::mutex mutex;
static std::condition_variable cond;
static std::set<std::tuple<int, int>> locked_tuples;

void lock(const std::tuple<int, int> & val)
{
    std::unique_lock<std::mutex> lk(mutex);
    while(locked_tuples.find(val) != locked_tuples.end())
    {
        cond.wait(lk);
    }
    locked_tuples.insert(val);
}

void unlock(const std::tuple<int, int> & val)
{
    std::unique_lock<std::mutex> lk(mutex);
    locked_tuples.erase(val);
    cond.notify_all();
}

void do_work(const std::tuple<int, int> & val)
{
    lock(val);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::stringstream ss;
    ss << "did work: {" << std::get<0>(val) << "," << std::get<1>(val) << "}" << std::endl;
    std::cout << ss.str() << std::flush;
    unlock(val);
}

int main()
{
    std::vector<std::tuple<int, int>> work;
    work.push_back(std::make_tuple(1,1));
    work.push_back(std::make_tuple(2,2));
    work.push_back(std::make_tuple(3,3));
    work.push_back(std::make_tuple(4,4)); // {4, 4}'s will contend
    work.push_back(std::make_tuple(4,4)); // {4, 4}'s will contend
    work.push_back(std::make_tuple(4,4)); // {4, 4}'s will contend
    work.push_back(std::make_tuple(4,4)); // {4, 4}'s will contend
    work.push_back(std::make_tuple(4,4)); // {4, 4}'s will contend
    work.push_back(std::make_tuple(5,5));
    work.push_back(std::make_tuple(6,6));

    std::vector<std::thread*> thread_list;
    for (std::tuple<int, int> t : work)
    {
        thread_list.push_back(new std::thread(&do_work,t));
    }

    for (std::thread * t : thread_list)
    {
        t->join();
    }
    for (std::thread * t : thread_list)
    {
        delete(t);
    }
    return 0;
}
