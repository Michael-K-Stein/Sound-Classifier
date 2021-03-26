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

class Data_Handler
{
private:
	std::vector<Data *> * data_vector; // all the data vectors
	std::vector<Data *> * training_data;
	std::vector<Data *> * validation_data;
	std::vector<Data *> * test_data;
	std::vector<Data *> * prediction_data;

	int original_data_size;
	int predict_data_size;

	int class_counts;
	int feature_vector_size;
	std::map<uint8_t, int> class_from_int;
	std::map<std::string, int> class_from_string;

	uint32_t max_data_vector_size = 1000000;

public:
	const double TRAIN_SET_PERCENT = 0.70;
	const double TEST_SET_PERCENT = 0.20;
	const double VALID_SET_PERCENT = 0.10;

	Data_Handler();		// ctor
	~Data_Handler();	// dtor

	void read_input_data_file(std::string file_path);
	void read_input_label_file(std::string file_path);

	void read_feature_vector(std::string filePath);
	void read_feature_labels(std::string filePath);
	void read_predict_feature_vector(std::string filePath);

	void split_data();
	void count_classes();
	void normalize_data();
	void normalize_prediction_data();
	void log();

	int get_class_count();
	int get_data_vector_size();
	int get_training_data_size();
	int get_test_data_size();
	int get_validation_data_size();
	int get_prediction_data_size();

	std::vector<Data *> * get_training_data();
	std::vector<Data *> * get_validation_data();
	std::vector<Data *> * get_test_data();
	std::vector<Data *> * get_prediction_data();
	std::map<uint8_t, int> get_class_map();

	uint32_t format(const unsigned char* bytes);
	static void print_loading(bool print_dots, int amt, int cap);

	void c_only();


};

