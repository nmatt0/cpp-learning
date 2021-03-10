#include <string>

namespace oop {
	class car {
	public:
		car() {
		}

		std::string Make() {
			return make;
		}

		std::string Model() {
			return model;
		}

		int Year() {
			return year;
		}
		virtual std::string Type() = 0;
		virtual std::string CarString() = 0;

	protected:
		std::string make;
		std::string model;
		int year;
		
	};

	class sedan : public car {
	public:
		sedan(std::string s_make, std::string s_model, int s_year) {
			make = s_make;
			model = s_model;
			year = s_year;
			type = "sedan";
		}
		
		std::string Type() override {
			return type;
		}

		std::string CarString() override {
			return std::string(std::to_string(year) + " " + make + " " + model + " (" + type + ")");
		}
	private:
		std::string type;
	};

	class pickup : public car {
	public:
		pickup(std::string p_make, std::string p_model, int p_year, int p_hauling_lbs) {
			make = p_make;
			model = p_model;
			year = p_year;
			type = "pickup";
			hauling_lbs = p_hauling_lbs;
		}
		
		std::string Type() override {
			return type;
		}

		std::string CarString() override {
			return std::string(std::to_string(year) + " " + make + " " + model + " (" + type + ")");
		}

		std::string HaulingLoad() {
			return std::string("Hauling Capacity: " + std::to_string(hauling_lbs));
		}
	private:
		std::string type;
		int hauling_lbs;
	};
} 
