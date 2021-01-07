#include "common_data.h"


template<class T>
common_data<T>::common_data()
{
}

template<class T>
common_data<T>::~common_data()
{
}

template<class T>
void common_data<T>::set_training_Data(std::vector<data<T> *> * vect) {
	training_data = vect;
}

template<class T>
void common_data<T>::set_test_Data(std::vector<data<T> *> * vect) {
	test_data = vect;
}

template<class T>
void common_data<T>::set_validation_Data(std::vector<data<T> *> * vect) {
	validation_data = vect;
}
