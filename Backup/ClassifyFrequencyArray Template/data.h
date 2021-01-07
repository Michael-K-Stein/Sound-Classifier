#ifndef __DATA_H
#define __DATA_H

#include "stdint.h"
#include "stdio.h"
#include <vector>


#pragma once
template <class T>
class data
{

	std::vector<uint8_t> * feature_vector;
	std::vector<T> * double_feature_vector;
	std::vector<T> *normalizedFeatureVector;
	std::vector<int> * class_vector;
	uint8_t label;
	int enum_label; // A: 1; B: 2
	double distance;

	public:
	data();
	~data();

	void set_feature_vector(std::vector<uint8_t> *);
	void set_double_feature_vector(std::vector<T> *);
	void setNormalizedFeatureVector(std::vector<T>*);
	void set_class_vector(int count);
	void append_to_feature_vector(uint8_t);
	void append_to_feature_vector(T);
	void set_label (uint8_t);
	void set_enumerated_label(int);
	void set_distance(T);

	T get_distance();
	int get_feature_vector_size();
	uint8_t get_label();
	uint8_t get_enumerated_label();
	std::vector<uint8_t> * get_feature_vector();
	std::vector<T> * get_double_feature_vector();
	std::vector<T> * getNormalizedFeatureVector();
	std::vector<int> * get_class_vector();

};

#endif