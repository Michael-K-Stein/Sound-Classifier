#include "data.h"

data::data()
{
	feature_vector = new std::vector<uint8_t>;
}


data::~data()
{
}

std::vector<double> * data::getNormalizedFeatureVector()
{
	return normalizedFeatureVector;
}
void data::setNormalizedFeatureVector(std::vector<double>* vect)
{
	normalizedFeatureVector = vect;
}

void data::set_feature_vector(std::vector<uint8_t> * vect) {
	feature_vector = vect;
}
 
void data::set_double_feature_vector(std::vector<double> * vect) {
	double_feature_vector = vect;
}

void data::create_feature_array() {
	if (feature_array) { free(feature_array); }
	feature_array = (uint8_t*)malloc(sizeof(uint8_t) * get_feature_vector_size());

	for (int i = 0; i < get_feature_vector_size(); i++) {
		feature_array[i] = feature_vector->at(i);
	}

}
uint8_t* data::get_feature_array() {
	return feature_array;
}

void data::set_class_vector(int count) {
	class_vector = new std::vector<int>();
	for (int i = 0; i < count; i++)
	{
		if (i == label)
			class_vector->push_back(1);
		else
			class_vector->push_back(0);
	}
}

void data::append_to_feature_vector(uint8_t val) {
	feature_vector->push_back(val);
}
void data::append_to_feature_vector(double val) {
	normalizedFeatureVector->push_back(val);
}

void data::set_label(uint8_t val) {
	label = val;
}

void data::set_enumerated_label(int val) {
	enum_label = val;
}

int data::get_feature_vector_size() {
	return feature_vector->size();
}

uint8_t data::get_label() {
	return label;
}

uint8_t data::get_enumerated_label() {
	return enum_label;
}

void data::set_distance(double val) {
	distance = val;
}

std::vector<uint8_t> * data::get_feature_vector() {
	return feature_vector;
}

std::vector<double> * data::get_double_feature_vector() {
	return double_feature_vector;
}

std::vector<int> * data::get_class_vector() {
	return class_vector;
}

double data::get_distance() {
	return distance;
}
