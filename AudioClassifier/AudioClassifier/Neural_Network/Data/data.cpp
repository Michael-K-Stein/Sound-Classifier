#include "data.h"

void Data::set_distance(double distance) {
	this->distance = distance;
}
void Data::set_feature_vector(std::vector<uint8_t> * vect) {
	this->feature_vector = vect;
}
void Data::set_normalized_feature_vector(std::vector<double> * norm_vect) {
	this->normalized_feature_vector = norm_vect;
}

void Data::set_class_vector(int counts) {
	this->class_vector = new std::vector<int>();
	for (int i = 0; i < counts; i++)
	{
		if (i == label)
			this->class_vector->push_back(1);
		else
			this->class_vector->push_back(0);
	}
}
void Data::set_class_array(int counts) {
	if (class_array != NULL) { free(class_array); }
	else { class_array = (int*)calloc(counts, sizeof(int)); }

	class_array[label] = 1;

	class_array_size = counts;
}

void Data::append_to_feature_vector(uint8_t val) {
	feature_vector->push_back(val);
}
void Data::append_to_feature_vector(double val) {
	normalized_feature_vector->push_back(val);
}
void Data::set_label(uint8_t label) {
	this->label = label;
}
void Data::set_enumerated_label(uint8_t label) {
	this->enumerated_label = label;
}

double Data::get_distance() {
	return this->distance;
}

int Data::get_feature_vector_size() {
	return feature_vector->size();
}
uint8_t Data::get_label() {
	return this->label;
}
uint8_t Data::get_enumerated_label() {
	return this->enumerated_label;
}

std::vector<uint8_t> * Data::get_feature_vector() {
	return this->feature_vector;
}
std::vector<double> * Data::get_normalized_feature_vector() {
	return this->normalized_feature_vector;
}
std::vector<int> * Data::get_class_vector() {
	return this->class_vector;
}
int * Data::get_class_array() {
	return this->class_array;
}

double * Data::get_normalized_feature_array() {
	return normalized_feature_array;
}

void Data::update_normalized_feature_array() {
	if (normalized_feature_array != NULL) { free(normalized_feature_array); }
	normalized_feature_array_size = normalized_feature_vector->size();
	normalized_feature_array = (double *)malloc(normalized_feature_array_size * sizeof(double));

	for (int i = 0; i < normalized_feature_array_size; i++) {
		normalized_feature_array[i] = normalized_feature_vector->at(i);
	}
}

int Data::get_feature_array_size() {
	return normalized_feature_array_size;
}

int Data::get_class_array_size() {
	return class_array_size;
}

void Data::c_only() {
	update_normalized_feature_array();

	if (class_array != NULL) { free(class_array); }
	class_array_size = class_vector->size();
	class_array = (int *)malloc(class_array_size * sizeof(int));
	for (int i = 0; i < class_array_size; i++) {
		class_array[i] = class_vector->at(i);
	}

	delete normalized_feature_vector;
	delete feature_vector;

	delete class_vector;
}