#ifndef __DATA_HANDLER_H
#define __DATA_HANDLER_H

#define FAST_DISTANCE

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
class data_handler
{

	std::vector<data *> * data_array;
	std::vector<data *> * training_data;
	std::vector<data *> * test_data;
	std::vector<data *> * validation_data;


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
	void split_data_fast();
	void count_classes();
	void normalize();

	uint32_t convert_to_little_endian(const unsigned char * bytes);

	int get_class_count();

	std::vector<data *> * get_training_data();
	std::vector<data *> * get_test_data();
	std::vector<data *> * get_validation_data();

	static void print_loading(bool print_dots, int amt, int cap);

	private:
	uint32_t max_image_array = 10000; // To keep the program from hogging the RAM
};

#endif