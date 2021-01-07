#include "data.h"

template<class T>
data<T>::data()
{
	feature_vector = new std::vector<uint8_t>;
}


template<class T> data<T>::~data()
{
}

template<class T> std::vector<T> * data<T>::getNormalizedFeatureVector()
{
	return normalizedFeatureVector;
}
template<class T> void data<T>::setNormalizedFeatureVector(std::vector<T>* vect)
{
	normalizedFeatureVector = vect;
}

template<class T> void data<T>::set_feature_vector(std::vector<uint8_t> * vect) {
	feature_vector = vect;
}
 
template<class T> void data<T>::set_double_feature_vector(std::vector<T> * vect) {
	double_feature_vector = vect;
}

template<class T> void data<T>::set_class_vector(int count) {
	class_vector = new std::vector<int>();
	for (int i = 0; i < count; i++)
	{
		if (i == label)
			class_vector->push_back(1);
		else
			class_vector->push_back(0);
	}
}

template<class T> void data<T>::append_to_feature_vector(uint8_t val) {
	feature_vector->push_back(val);
}
template<class T> void data<T>::append_to_feature_vector(T val) {
	normalizedFeatureVector->push_back(val);
}

template<class T> void data<T>::set_label(uint8_t val) {
	label = val;
}

template<class T> void data<T>::set_enumerated_label(int val) {
	enum_label = val;
}

template<class T> int data<T>::get_feature_vector_size() {
	return feature_vector->size();
}

template<class T> uint8_t data<T>::get_label() {
	return label;
}

template<class T> uint8_t data<T>::get_enumerated_label() {
	return enum_label;
}

template<class T> void data<T>::set_distance(T val) {
	distance = val;
}

template<class T> std::vector<uint8_t> * data<T>::get_feature_vector() {
	return feature_vector;
}

template<class T> std::vector<T> * data<T>::get_double_feature_vector() {
	return double_feature_vector;
}

template<class T> std::vector<int> * data<T>::get_class_vector() {
	return class_vector;
}

template<class T> T data<T>::get_distance() {
	return distance;
}
