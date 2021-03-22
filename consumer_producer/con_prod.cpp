#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>
#include <atomic>

std::queue<int> q;
std::mutex m;
std::mutex m_cout; // had to lock using cout to get consistent output. better way? (see consumer function)
std::condition_variable cv;
volatile bool done; // does this need to be volatile???
std::atomic<int> producer_count; // does this need to be atomic???

class job {
	public:
	job(int v, bool s)
	{
		value = v;
		status = s;
	}
	
	int value;
	bool status;
};

job * consumer_job()
{
	std::unique_lock<std::mutex> lk(m);
	while(q.empty())
	{
		cv.wait(lk); // one consumer was getting stuck here before adding cv.notify_all() in consumer()
		if (q.empty() && done)
		{
			return new job(0,false);
		}
	}
	// I think I need this block here in the case that notify_all get called ????
	if (q.empty() && done)
	{
		return new job(0,false);
	}
	// is there a queue object where the pop() function returns a value?
	int val = q.front();
	q.pop();
	return new job(val,true);
}

void producer_job(int val)
{
	std::unique_lock<std::mutex> lk(m);
	q.push(val);
	cv.notify_one();
}

void consumer(int cid)
{
	while(!done) // should done be volatile ???
	{
		job * j = consumer_job();
		if (!j->status) {
			return;
		}

		std::unique_lock<std::mutex> lk(m_cout);
		std::cout << "(" << cid << ") " << j->value << std::endl << std::flush;
	}
	cv.notify_all();
}

void producer(int start, int end)
{
	// does producer_count need to be an atomic variable? use a mutex instead?
	producer_count++;
	for (int i = start; i < end+1; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		producer_job(i);
	}
	producer_count--;
	if (producer_count == 0)
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
	std::thread c3(&consumer,3);
	std::thread c4(&consumer,4);
	std::thread p1(&producer,1,100);
	std::thread p2(&producer,1000,1100);

	consumers.push_back(&c1);
	consumers.push_back(&c2);
	consumers.push_back(&c3);
	consumers.push_back(&c4);
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
