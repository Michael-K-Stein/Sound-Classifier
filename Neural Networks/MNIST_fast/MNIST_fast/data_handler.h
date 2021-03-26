#pragma once

#include "fstream"
#include "stdint.h"
#include "data.h"
#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include <math.h>
#include <algorithm>
#include <random>

class data_handler
{
private:
	std::vector<data *> * data_vector; // all the data vectors
	std::vector<data *> * training_data;
	std::vector<data *> * validation_data;
	std::vector<data *> * test_data;
	
	int class_counts;
	int feature_vector_size;
	std::map<uint8_t, int> class_from_int;
	std::map<std::string, int> class_from_string;

	uint32_t max_data_vector_size = 6000;

public:
	const double TRAIN_SET_PERCENT = 0.70;
	const double TEST_SET_PERCENT = 0.20;
	const double VALID_SET_PERCENT = 0.10;

	data_handler();		// ctor
	~data_handler();	// dtor

	void read_input_data_file(std::string file_path);
	void read_input_label_file(std::string file_path);

	void split_data();
	void count_classes();
	void normalize_data();
	void log();

	int get_class_count();
	int get_data_vector_size();
	int get_training_data_size();
	int get_test_data_size();
	int get_validation_data_size();

	std::vector<data *> * get_training_data();
	std::vector<data *> * get_validation_data();
	std::vector<data *> * get_test_data();
	std::map<uint8_t, int> get_class_map();

	uint32_t format(const unsigned char* bytes);
};

