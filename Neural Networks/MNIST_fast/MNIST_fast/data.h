#pragma once

#include <vector>
#include <stdint.h>
#include <stdio.h>

class data
{
private:
	std::vector<uint8_t> * feature_vector;
	std::vector<double> * normalized_feature_vector;
	double * normalized_feature_array;
	uint32_t normalized_feature_array_size;
	std::vector<int> * class_vector;
	int * class_array;
	int class_array_size;
	uint8_t label;
	uint8_t enumerated_label;
	double distance;
public:
	void set_distance(double distance);
	void set_feature_vector(std::vector<uint8_t> * vect);
	void set_normalized_feature_vector(std::vector<double> * norm_vect);
	void update_normalized_feature_array();
	void set_class_vector(int counts);
	void set_class_array(int counts);
	void append_to_feature_vector(uint8_t val);
	void append_to_feature_vector(double val);
	void set_label(uint8_t label);
	void set_enumerated_label(uint8_t label);

	double get_distance();
	int get_feature_vector_size();
	int get_feature_array_size();
	uint8_t get_label();
	uint8_t get_enumerated_label();

	std::vector<uint8_t> * get_feature_vector();
	std::vector<double> * get_normalized_feature_vector();
	double * get_normalized_feature_array();
	std::vector<int> * get_class_vector();
	int * get_class_array();
	int get_class_array_size();

	void c_only();
};

