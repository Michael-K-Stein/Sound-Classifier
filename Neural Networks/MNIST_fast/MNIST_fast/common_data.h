#pragma once

#include "data.h"
#include <vector>

class common_data
{
protected:
	std::vector<data *> * training_data;
	std::vector<data *> * validation_data;
	std::vector<data *> * test_data;
public:
	void set_training_data(std::vector<data *> * vect);
	void set_validation_data(std::vector<data *> * vect);
	void set_test_data(std::vector<data *> * vect);

	std::vector<data *> * get_validation_data();
};

