#include "sensor.h"
#include <iostream>
#include <vector>

int main() {

	std::vector<sensor> sensor_list;
	sensor s1;
	sensor s2;
	sensor s3;
	sensor_list.push_back(s1);
	sensor_list.push_back(s2);
	sensor_list.push_back(s3);
	
	// start threads
	for (sensor s : sensor_list) {
		std::cout << 1 << std::endl;
		s.start();
	}

	// stop threads
	for (sensor s : sensor_list) {
		std::cout << 2 << std::endl;
		s.stop();
	}

	// stop threads
	for (sensor s : sensor_list) {
		std::cout << "temp reading: " << s.get_temp() << std::endl;
	}

	return 0;
}
