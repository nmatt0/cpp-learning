#include "car.h"
#include <iostream>

using namespace oop;

	int main() {

		car * c1 = nullptr;
		car * c2 = nullptr;
		pickup * p2 = nullptr;
		pickup * p3 = nullptr;

		c1 = new sedan("Honda","Civic",2020);
		c2 = new pickup("Ford","F150",2021,5000);
		p3 = new pickup("Ford","F150",2021,5000);
		p2 = (pickup*) &c2;

		std::cout << c1->CarString() << std::endl << std::endl;

		std::cout << c2->CarString() << std::endl;
		std::cout << p2->HaulingLoad() << std::endl << std::endl; // Why does this print 0?? clearly some kind of issue casting pointer to pickup

		std::cout << p3->CarString() << std::endl;
		std::cout << p3->HaulingLoad() << std::endl << std::endl;
		return 0;
	}

