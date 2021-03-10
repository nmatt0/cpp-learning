#include "car.h"
#include <iostream>
#include <cassert>

using namespace oop;

// factory
std::unique_ptr<car> car::create_car(std::string make, std::string model, int year, std::string type) {
	car * p = nullptr;
	if (type == "sedan") {
		p = new sedan(make, model, year);
	}
	if (type == "pickup") {
		p = new pickup(make, model, year, 0);
	}
	assert(p);
	return std::unique_ptr<car>(p);
}

int main() {

	car * c1 = nullptr;
	car * c2 = nullptr;
	pickup * p2 = nullptr;
	pickup * p3 = nullptr;

	c1 = new sedan("Honda","Civic",2020);
	c2 = new pickup("Ford","F150",2021,5000);
	p3 = new pickup("Ford","F150",2021,5000);
	// p2 = (pickup*)(c2); // explicit downcast
	p2 = dynamic_cast<pickup*>(c2); // safe downcast
	assert(p2); // dynamic_cast will return nullptr if the cast is not possible

	std::cout << c1->CarString() << std::endl << std::endl;

	std::cout << c2->CarString() << std::endl;
	std::cout << p2->HaulingLoad() << std::endl << std::endl;

	std::cout << p3->CarString() << std::endl;
	std::cout << p3->HaulingLoad() << std::endl << std::endl;

	// using factory
	std::unique_ptr<car> cc1 = std::move(car::create_car("Honda","Civic",2020,"sedan"));
	std::cout << "using factory:" << std::endl;
	std::cout << cc1->CarString() << std::endl << std::endl;

	return 0;
}

