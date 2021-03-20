#include "sensor.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <memory>
#include <mutex>
#include <csignal>

std::mutex sensor::i2c_sensor_read_mutex;
std::vector<sensor*> sensor_list;

static void shutdown_handler(int signal)
{
	for (auto & s : sensor_list)
	{
		s->shutdown();
	}
}

int main() {
	time_t t;
	srand((unsigned) time(&t));
	
	sensor_list.push_back(new sensor(t,1));
	sensor_list.push_back(new sensor(t,2));
	sensor_list.push_back(new sensor(t,3));
	
	// start threads
	for (auto & s : sensor_list) {
		s->start();
	}

	signal(SIGINT, shutdown_handler);
	signal(SIGTERM, shutdown_handler);
	signal(SIGPIPE, SIG_IGN);

	// wait on threads
	for (auto & s : sensor_list) {
		s->wait();
	}

	return 0;
}
