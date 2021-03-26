#include "data.h"

void data::set_distance(double distance) {
	this->distance = distance;
}
void data::set_feature_vector(std::vector<uint8_t> * vect) {
	this->feature_vector = vect;
}
void data::set_normalized_feature_vector(std::vector<double> * norm_vect) {
	this->normalized_feature_vector = norm_vect;
}

void data::set_class_vector(int counts) {
	this->class_vector = new std::vector<int>();
	for (int i = 0; i < counts; i++)
	{
		if (i == label)
			this->class_vector->push_back(1);
		else
			this->class_vector->push_back(0);
	}
}
void data::set_class_array(int counts) {
	if (class_array != NULL) { free(class_array); }
	else { class_array = (int*)calloc(counts, sizeof(int)); }
	
	class_array[label] = 1;

	class_array_size = counts;
}

void data::append_to_feature_vector(uint8_t val) {
	feature_vector->push_back(val);
}
void data::append_to_feature_vector(double val) {
	normalized_feature_vector->push_back(val);
}
void data::set_label(uint8_t label) {
	this->label = label;
}
void data::set_enumerated_label(uint8_t label) {
	this->enumerated_label = label;
}

double data::get_distance() {
	return this->distance;
}

int data::get_feature_vector_size() {
	return feature_vector->size();
}
uint8_t data::get_label() {
	return this->label;
}
uint8_t data::get_enumerated_label() {
	return this->enumerated_label;
}

std::vector<uint8_t> * data::get_feature_vector() {
	return this->feature_vector;
}
std::vector<double> * data::get_normalized_feature_vector() {
	return this->normalized_feature_vector;
}
std::vector<int> * data::get_class_vector() {
	return this->class_vector;
}
int * data::get_class_array() {
	return this->class_array;
}

double * data::get_normalized_feature_array() {
	return normalized_feature_array;
}

void data::update_normalized_feature_array() {
	if (normalized_feature_array != NULL) { free(normalized_feature_array); }
	normalized_feature_array_size = normalized_feature_vector->size();
	normalized_feature_array = (double *)malloc(normalized_feature_array_size * sizeof(double));

	for (int i = 0; i < normalized_feature_vector->size(); i++) {
		normalized_feature_array[i] = normalized_feature_vector->at(i);
	}
}

int data::get_feature_array_size() {
	return normalized_feature_array_size;
}

int data::get_class_array_size() {
	return class_array_size;
}

void data::c_only() {
	update_normalized_feature_array();

	if (class_array != NULL) { free(class_array); }
	class_array = (int *)malloc(class_vector->size() * sizeof(int));
	for (int i = 0; i < class_vector->size(); i++) {
		class_array[i] = class_vector->at(i);
	}

	delete normalized_feature_vector;
	delete feature_vector;

	delete class_vector;
}