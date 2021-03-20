#include <thread>
#include <mutex>
#include <chrono>
#include <cassert>
#include <iostream>


class sensor {
public:

	sensor(time_t seed, int tid) : rand_seed(seed), thread_id(tid)
    {
	}

	virtual ~sensor() = default;

	void reading_thread()
	{
		while(!should_stop)
		{
			{
				std::unique_lock<std::mutex> lock(i2c_sensor_read_mutex);
				// this simulates a read from the i2c sensor
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				temp = rand() % 100;
				std::cout << "temp (" << thread_id << "): " << temp << std::endl;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}

	void start()
    {
		assert(!sensor_t);
		sensor_t = new std::thread(&sensor::reading_thread, this);
	}

	void shutdown()
	{
		should_stop = true;
	}

	void wait()
	{
		assert(sensor_t);
		sensor_t->join();
		delete (sensor_t);
		sensor_t = nullptr;
	}

	int get_temp() {
		return temp;
	}

private:
	bool should_stop = false;
	int temp = 0;
	time_t rand_seed;
	int thread_id;
	std::thread * sensor_t = nullptr;
    static std::mutex i2c_sensor_read_mutex; // mutex shared between all threads
};
