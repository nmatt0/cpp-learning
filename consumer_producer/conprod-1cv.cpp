#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>
#include <atomic>
#include <sstream>

static std::queue<int> q;
static std::mutex mutex;
static std::condition_variable cv;
volatile bool done = false;
static const int MAX_Q_SIZE = 10;
static std::atomic<int> producer_count;

bool consumer_job(int & val)
{
	std::unique_lock<std::mutex> lk(mutex);
	while(q.empty() && !done)
	{
		cv.wait_for(lk,std::chrono::milliseconds(500));
		if (q.empty() && done)
		{
			return false;
		}
	}
	if (q.empty() && done)
	{
		return false;
	}
	val = q.front();
	q.pop();
	cv.notify_one();
	return true;
}

void producer_job(int val)
{
	std::unique_lock<std::mutex> lk(mutex);
	while (q.size() >= MAX_Q_SIZE)
	{
		cv.wait(lk);
	}
	q.push(val);
	cv.notify_one();
}

void consumer(int cid)
{
	while(!done)
	{
		int val;
		if (!consumer_job(val)) {
			return;
		}
		std::stringstream ss;
		ss << "(" << cid << ") " << val << std::endl;
		std::cout << ss.str() << std::flush;
	}
}

void producer(int start, int end)
{
	producer_count++;
	for (int i = start; i < end+1; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		producer_job(i);
	}
	producer_count--;
	if (producer_count <= 0)
	{
		done = true;
	}
}

int main ()
{
	done = false;
	producer_count = 0;

	std::vector<std::thread*> consumers;
	std::vector<std::thread*> producers;

	std::thread c1(&consumer,1);
	std::thread c2(&consumer,2);
	std::thread p1(&producer,1,100);
	std::thread p2(&producer,1000,1100);

	consumers.push_back(&c1);
	consumers.push_back(&c2);
	producers.push_back(&p1);
	producers.push_back(&p2);

	for (std::thread * c : consumers)
	{
		c->join();
	}
	for (std::thread * c : producers)
	{
		c->join();
	}

}
