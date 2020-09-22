#include "common_data.h"



common_data::common_data()
{
}


common_data::~common_data()
{
}

void common_data::set_training_Data(std::vector<data *> * vect) {
	training_data = vect;
}

void common_data::set_test_Data(std::vector<data *> * vect) {
	test_data = vect;
}

void common_data::set_validation_Data(std::vector<data *> * vect) {
	validation_data = vect;
}

void common_data::append_training_data(data * d) {
	training_data->push_back(d);
}