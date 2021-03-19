#include "sensor.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <memory>
#include <mutex>

std::mutex sensor::i2c_sensor_read_mutex;

int main() {
	time_t t;
	srand((unsigned) time(&t));
	std::vector<std::unique_ptr<sensor>> sensor_list;
	std::unique_ptr<sensor> p1(new sensor(t));
	std::unique_ptr<sensor> p2(new sensor(t));
	std::unique_ptr<sensor> p3(new sensor(t));
	sensor_list.push_back(std::move(p1));
	sensor_list.push_back(std::move(p2));
	sensor_list.push_back(std::move(p3));
	
	// start threads
	for (auto & s : sensor_list) {
		s->start();
	}

	//// stop threads
	for (auto & s : sensor_list) {
		s->stop();
	}

	//// stop threads
	for (auto & s : sensor_list) {
		std::cout << "temp reading: " << s->get_temp() << std::endl;
	}

	return 0;
}
