#pragma once

#include "data.h"
#include <vector>

class Common_data
{
protected:
	std::vector<Data *> * training_data;
	std::vector<Data *> * validation_data;
	std::vector<Data *> * test_data;
public:
	void set_training_data(std::vector<Data *> * vect);
	void set_validation_data(std::vector<Data *> * vect);
	void set_test_data(std::vector<Data *> * vect);

	std::vector<Data *> * get_validation_data();
};

