#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::queue<int> q;
std::mutex m;
std::condition_variable cv;
bool done;

int consumer_job()
{
	std::unique_lock<std::mutex> lk(m);
	while(q.empty() && !done)
	{
		cv.wait(lk);
	}
	// is there a queue object where the pop() function returns a value?
	int val = q.front();
	q.pop();
	return val;
}

void producer_job(int val)
{
	std::unique_lock<std::mutex> lk(m);
	q.push(val);
	cv.notify_one();
}

void consumer(int cid)
{
	int val;
	while(!done)
	{
		val = consumer_job();
		std::cout << "(" << cid << ") " << val << std::endl;
	}
}

void producer()
{
	for (int i = 0; i < 100; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		producer_job(i);
	}
	done = true;
}

int main ()
{
	done = false;
	// create threads
	std::thread c1(&consumer,1);
	std::thread c2(&consumer,2);
	std::thread p1(&producer);
	// join threads
	c1.join();
	c2.join();
	p1.join();
}
