#ifndef __COMMON_HPP
#define __COMMON_HPP


#pragma once

#include <vector>
#include "data.h"
#include "data_handler.h"

class common_data
{

	public:
	common_data();
	~common_data();
	void set_training_Data(std::vector<data *> * vect);
	void set_test_Data(std::vector<data *> * vect);
	void set_validation_Data(std::vector<data *> * vect);


	protected:
	std::vector <data *> * training_data;
	std::vector <data *> * test_data;
	std::vector <data *> * validation_data;

};


#endif // !__COMMON_HPP