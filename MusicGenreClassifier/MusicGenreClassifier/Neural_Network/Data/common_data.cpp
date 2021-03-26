#include "common_data.h"

void Common_data::set_training_data(std::vector<Data *> * vect) {
	training_data = vect;
}
void Common_data::set_validation_data(std::vector<Data *> * vect) {
	validation_data = vect;
}
void Common_data::set_test_data(std::vector<Data *> * vect) {
	test_data = vect;
}

std::vector<Data *> * Common_data::get_validation_data() {
	return validation_data;
}