#ifndef __DATA_HANDLER_H
#define __DATA_HANDLER_H
#pragma once

#include <fstream>
#include "stdint.h"
#include "data.h"
#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include <algorithm> 

class data_handler
{
public:

	std::vector<data *> * data_array;
	std::vector<data *> * training_data;
	std::vector<data *> * test_data;
	std::vector<data *> * validation_data;

	int num_classes;
	int feature_vector_size;
	std::map<uint8_t, int> class_map;

	const double TRAIN_SET_PERCENT = 0.75;
	const double TEST_SET_PERCENT = 0.0;
	const double VALIDATION_PERCENT = 0.25;


	data_handler();
	~data_handler();

	void read_feature_vector(std::string filePath);
	void read_feature_labels(std::string filePath);
	void split_data();
	void count_classes();

	uint32_t convert_to_uint32(const unsigned char * bytes);

	std::vector<data *> * get_training_data();
	std::vector<data *> * get_test_data();
	std::vector<data *> * get_validation_data();

private:
	uint32_t max_image_array = 1000;
};

#endif