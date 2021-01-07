#ifndef __COMMON_HPP
#define __COMMON_HPP


#pragma once

#include <vector>
#include "data.h"
#include "data_handler.h"

template<class T>
class common_data
{

	public:
	common_data();
	~common_data();
	void set_training_Data(std::vector<data<T> *> * vect);
	void set_test_Data(std::vector<data<T> *> * vect);
	void set_validation_Data(std::vector<data<T> *> * vect);


	protected:
	std::vector <data<T> *> * training_data;
	std::vector <data<T> *> * test_data;
	std::vector <data<T> *> * validation_data;

};


#endif // !__COMMON_HPP