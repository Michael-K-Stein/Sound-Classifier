#include "knn.h"
#include <cmath>
#include <limits>
#include <map>
#include "stdint.h"
#include "data_handler.h"

#include "Graphic.h"

template<class T>
knn<T>::knn(int val) {
	k = val;
}
template<class T>
knn<T>::knn() {}
template<class T>
knn<T>::~knn() {}

template<class T>
void knn<T>::find_knearest(data<T> * query_point) {
	neighbors = new std::vector<data<T> *>;
	T min = std::numeric_limits<T>::max();
	T previous_min = min;
	int index = 0;

	for (int i = 0; i < k; i++) {
		if (i == 0) {
			for (int j = 0; j < training_data->size(); j++) {
				T distance = calculate_distance(query_point, training_data->at(j));
				training_data->at(j)->set_distance(distance);
				if (distance < min) {
					min = distance;
					index = j;
				}
			}
			neighbors->push_back(training_data->at(index));
			previous_min = min;
			min = std::numeric_limits<T>::max();
		}
		else {
			for (int j = 0; j < training_data->size(); j++) {
				T distance = training_data->at(j)->get_distance(); // calculate_distance(query_point, training_data->at(j));
				if (distance > previous_min && distance < min) {
					min = distance;
					index = j;
				}
			}
			neighbors->push_back(training_data->at(index));
			previous_min = min;
			min = std::numeric_limits<T>::max();
		}
	}
}
/*
void knn::set_training_Data(std::vector<data<T> *> * vect) {
	training_data = vect;
}

void knn::set_test_Data(std::vector<data<T> *> * vect) {
	test_data = vect;
}

void knn::set_validation_Data(std::vector<data<T> *> * vect) {
	validation_data = vect;
}
*/  // Now in common_data
template<class T>
void knn<T>::set_k(int val) {
	k = val;
}

template<class T>
int knn<T>::predict() {
	std::map<uint8_t, int> class_freq;
	for (int i = 0; i < neighbors->size(); i++) { // Count the frequency of a class in the neighboring points
		if (class_freq.find(neighbors->at(i)->get_label()) == class_freq.end()) {
			class_freq[neighbors->at(i)->get_label()] = 1;
		} else {
			class_freq[neighbors->at(i)->get_label()]++;
		}
	}

	int best = 0;
	int max = 0;

	for (std::pair<uint8_t, int> kv : class_freq) { // Find the most frequent class
		if (kv.second > max) {
			max = kv.second;
			best = kv.first;
		}
	}

	neighbors->clear();
	return best;
}

template<class T>
inline T knn<T>::calculate_distance_fast(data<T> * query_point, data<T> * input) {
	float distance = 0.0;
	if (query_point->get_feature_vector_size() != input->get_feature_vector_size()) {
		printf("Vector size mismatch");
		exit(1);
	}
	//#ifdef EUCLID

	for (uint16_t i = 0; i < query_point->get_feature_vector_size(); i++) {
		distance += pow(query_point->get_feature_vector()->at(i) - input->get_feature_vector()->at(i), 2);
	}
	//distance = sqrt(distance); //Not neccessary for comparisons
/*#elif defined MANHATTAN
	// Not implemented
#endif */
	return distance;
}

template<class T>
T knn<T>::calculate_distance(data<T> * query_point, data<T> * input) {
	T distance = 0.0;
	if (query_point->get_feature_vector_size() != input->get_feature_vector_size()) {
		printf("Vector size mismatch");
		exit(1);
	}
//#ifdef EUCLID

	for (unsigned i = 0; i < query_point->get_feature_vector_size(); i++) {
		distance += pow(query_point->get_feature_vector()->at(i) - input->get_feature_vector()->at(i),2);
	}
	distance = sqrt(distance); //Not neccessary for comparisons
/*#elif defined MANHATTAN
	// Not implemented
#endif */
	return distance;
}
template<class T>
T knn<T>::validate_performance() {
	double current_performance = 0;
	int count = 0;
	int data_index = 0;

	for (data<T> * query_point : * validation_data) {
		find_knearest(query_point);
		int prediction = predict();
		//int asciiPred = prediction + 48;
		//Graphic::drawImage(query_point, &asciiPred);
		printf("Guessed %d for %d\n", prediction, query_point->get_label());
		if (prediction == query_point->get_label()) {
			count++;
		}
		else {
			//printf("Mistaked %d for %d\n", query_point->get_label(), prediction);
		}
		data_index++;
		
		printf("Current Performance: %.3f %%\n", ((double)count*100.0) / ((double)data_index));
	}

	current_performance = ((double)count*100.0) / ((double)validation_data->size());
	printf("Validation Performance for K = %d: %.3f %%\n", k, current_performance);
	return current_performance;
}

template<class T>
T knn<T>::test_performance() {
	double current_performance = 0;
	int count = 0;
	//int data_index = 0;

	// A map to count the amount of times different mistakes were made.
	std::map<std::tuple<int, int>, int> mistakes; // < PREDICTION , LABEL , COUNT >

	for (data<T> * query_point : * test_data) {
		find_knearest(query_point);
		int prediction = predict();
		if (prediction == query_point->get_label()) {
			count++;
		}
		else {
			std::tuple<int, int> mistakeInd = std::tuple<int, int>(prediction, query_point->get_label());
			if (mistakes.find(mistakeInd) == mistakes.end()) {
				mistakes[mistakeInd] = 1;
			}
			else {
				mistakes[mistakeInd]++;
			}
		}
		//data_index++;
		//printf("Current Performance: %.3f %%\n", ((double)count*100.0) / ((double)data_index));
	}

	// Show mistakes
	for (std::map<std::tuple<int, int>, int>::iterator it = mistakes.begin(); it != mistakes.end(); it++)
	{
		printf("Guessed '%d' for '%d' %d times.\n", std::get<0>(it->first), std::get<1>(it->first), it->second);
	}

	current_performance = ((double)count*100.0) / ((double)test_data->size());
	printf("Test Performance: %.3f %%\n", current_performance);
	return current_performance;
}

// Function used for real predictions
template<class T>
int knn<T>::real_predict() {
	// A map to predictions and the amout of times they were made
	std::map<int, int> predictions;

	for (data<T> * query_point : *test_data) {
		find_knearest(query_point);
		int prediction = predict();
		if (predictions.find(prediction) == predictions.end()) 
		{ predictions[prediction] = 1; }
		else { predictions[prediction]++; }
	}

	int top_guessed = predictions.begin()->first;
	int top_guessed_count = 0;

	printf("Prediction table: \n");
	for (auto tpl : predictions) {
		printf("\t%u) %.2f%%\n", tpl.first, tpl.second / test_data->size());
		if (tpl.second > top_guessed_count) {
			top_guessed = tpl.first;
		}
	}

	return top_guessed;
}