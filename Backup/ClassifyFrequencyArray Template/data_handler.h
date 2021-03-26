#ifndef __DATA_HANDLER_H
#define __DATA_HANDLER_H

#include <fstream>
#include "stdint.h"
#include "data.h"
#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include <algorithm> 
#include "Graphic.h"

#pragma once
template<class T>
class data_handler
{

	std::vector<data<T> *> * data_array;
	std::vector<data<T> *> * training_data;
	std::vector<data<T> *> * test_data;
	std::vector<data<T> *> * validation_data;


	int num_classes;
	int feature_vector_size;
	int original_data_size;
	int predict_data_size;
	std::map<uint8_t, int> class_map;
	std::map<std::string, int> classMap;

	const double TRAIN_SET_PERCENT = 0.75;
	const double TEST_SET_PERCENT = 0.20;
	const double VALIDATION_PERCENT = 0.05;

	public:
	data_handler();
	~data_handler();

	void read_csv(std::string path, std::string delimiter);
	void read_feature_vector(std::string filePath);
	void read_feature_labels(std::string filePath);
	void read_predict_feature_vector(std::string filePath);
	void read_predict_feature_labels(std::string filePath);
	void split_data();
	void count_classes();
	void normalize();

	uint32_t convert_to_little_endian(const unsigned char * bytes);

	int get_class_count();

	std::vector<data<T> *> * get_training_data();
	std::vector<data<T> *> * get_test_data();
	std::vector<data<T> *> * get_validation_data();

	private:
	uint32_t max_image_array = 10000; // To keep the program from hogging the RAM
};

#endif