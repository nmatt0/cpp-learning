#include <thread>
#include <mutex>
#include <ctime>
#include <chrono>
#include <cassert>

std::mutex i2c_sensor_read_mutex;

class sensor {
public:

	sensor() {
	}

	virtual ~sensor() = default;

	void reading_thread() {
		srand((unsigned) time(0));
		
		{
			std::unique_lock<std::mutex> lock(i2c_sensor_read_mutex);
			// this simulates a read from the i2c sensor
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			temp = rand() % 100;
		}
	}

	void start() {
		assert(!sensor_t);
		sensor_t = new std::thread(&sensor::reading_thread, this);
	}

	void stop() {
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
	std::thread * sensor_t = nullptr;
};
