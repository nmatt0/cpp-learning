#include <thread>
#include <mutex>
#include <chrono>
#include <cassert>


class sensor {
public:

	sensor(time_t seed) : rand_seed(seed)
    {
	}

	virtual ~sensor() = default;

    sensor(const sensor & other) : sensor_t(other.sensor_t), temp(other.temp), rand_seed(other.rand_seed)
	{
	}

	void reading_thread()
	{
		{
			std::unique_lock<std::mutex> lock(i2c_sensor_read_mutex);
			// this simulates a read from the i2c sensor
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			temp = rand() % 100;
		}
	}

	void start()
    {
		assert(!sensor_t);
		sensor_t = new std::thread(&sensor::reading_thread, this);
	}

	void stop()
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
	int temp = 0;
	time_t rand_seed;
	std::thread * sensor_t = nullptr;
    static std::mutex i2c_sensor_read_mutex; // mutex shared between all threads
};
