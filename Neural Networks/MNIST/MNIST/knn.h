#ifndef __KNN_H
#define __KNN_H
#pragma once

#include "common_data.h"

class knn : public common_data {

	int k;
	std::vector <data *> * neighbors;
	/*std::vector <data *> * training_data;
	std::vector <data *> * test_data;
	std::vector <data *> * validation_data;*/
	
	public:
	knn(int);
	knn();
	~knn();

	void find_knearest(data * query_point);
	/*void set_training_Data(std::vector<data *> * vect);
	void set_test_Data(std::vector<data *> * vect);
	void set_validation_Data(std::vector<data *> * vect);*/
	void set_k(int val);

	int predict();
	double calculate_distance(data * query_point, data * input);
	double validate_performance();
	double test_performance();
};



#endif // !__KNN_H