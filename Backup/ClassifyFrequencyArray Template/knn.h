#ifndef __KNN_H
#define __KNN_H
#pragma once

#include "common_data.h"

template<class T>
class knn : public common_data<T> {

	int k;
	std::vector <data<T> *> * neighbors;
	std::vector <data<T> *> * training_data;
	std::vector <data<T> *> * test_data;
	std::vector <data<T> *> * validation_data;  // Now in common_data
	
	public:
	knn(int);
	knn();
	~knn();

	void find_knearest(data<T> * query_point);
	/*void set_training_Data(std::vector<data<T> *> * vect);
	void set_test_Data(std::vector<data<T> *> * vect);
	void set_validation_Data(std::vector<data<T> *> * vect);*/  // Now in common_data
	void set_k(int val);

	int predict();
	T calculate_distance(data<T> * query_point, data<T> * input);
	T calculate_distance_fast(data<T> * query_point, data<T> * input);
	T validate_performance();
	T test_performance();
	int real_predict();
};



#endif // !__KNN_H