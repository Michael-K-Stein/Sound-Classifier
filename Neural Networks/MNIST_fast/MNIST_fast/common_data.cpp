#include "common_data.h"

void common_data::set_training_data(std::vector<data *> * vect) {
	training_data = vect;
}
void common_data::set_validation_data(std::vector<data *> * vect) {
	validation_data = vect;
}
void common_data::set_test_data(std::vector<data *> * vect) {
	test_data = vect;
}

std::vector<data *> * common_data::get_validation_data() {
	return validation_data;
}