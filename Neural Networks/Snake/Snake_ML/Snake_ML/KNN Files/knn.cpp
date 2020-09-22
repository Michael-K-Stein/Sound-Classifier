#include "knn.h"
#include <cmath>
#include <limits>
#include <map>
#include "stdint.h"

knn::knn(int val) {
	k = val;
}

knn::knn() {}
knn::~knn() {}

void knn::find_knearest(data * query_point) {
	neighbors = new std::vector<data *>;
	double min = std::numeric_limits<double>::max();
	double previous_min = min;
	int index = 0;

	for (int i = 0; i < k; i++) {
		if (i == 0) {
			for (int j = 0; j < training_data->size(); j++) {
				double distance = calculate_distance(query_point, training_data->at(j));
				training_data->at(j)->set_distance(distance);
				if (distance < min) {
					min = distance;
					index = j;
				}
			}
			neighbors->push_back(training_data->at(index));
			previous_min = min;
			min = std::numeric_limits<double>::max();
		}
		else {
			for (int j = 0; j < training_data->size(); j++) {
				double distance = training_data->at(j)->get_distance(); // calculate_distance(query_point, training_data->at(j));
				if (distance > previous_min && distance < min) {
					min = distance;
					index = j;
				}
			}
			neighbors->push_back(training_data->at(index));
			previous_min = min;
			min = std::numeric_limits<double>::max();
		}
	}
}
/*
void knn::set_training_Data(std::vector<data *> * vect) {
	training_data = vect;
}

void knn::set_test_Data(std::vector<data *> * vect) {
	test_data = vect;
}

void knn::set_validation_Data(std::vector<data *> * vect) {
	validation_data = vect;
}
*/  // Now in common_data
void knn::set_k(int val) {
	k = val;
}

int knn::predict() {
	std::map<uint8_t, int> class_freq;
	for (int i = 0; i < neighbors->size(); i++) { // Count the frequency of a class in the neighboring points
		if (class_freq.find(neighbors->at(i)->getLabel()) == class_freq.end()) {
			class_freq[neighbors->at(i)->getLabel()] = 1;
		} else {
			class_freq[neighbors->at(i)->getLabel()]++;
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

double knn::calculate_distance(data * query_point, data * input) {
	double distance = 0.0;
	if (query_point->getNormalizedFeatureVector()->size() != input->getNormalizedFeatureVector()->size()) {
		printf("Vector size mismatch");
		exit(1);
	}
//#ifdef EUCLID

	for (unsigned i = 0; i < query_point->getNormalizedFeatureVector()->size(); i++) {
		distance += pow(query_point->getNormalizedFeatureVector()->at(i) - input->getNormalizedFeatureVector()->at(i),2);
	}
	distance = sqrt(distance);
/*#elif defined MANHATTAN
	// Not implemented
#endif */
	return distance;
}
double knn::validate_performance() {
	double current_performance = 0;
	int count = 0;
	int data_index = 0;

	for (data * query_point : * validation_data) {
		find_knearest(query_point);
		int prediction = predict();
		int asciiPred = prediction + 48;
		//Graphic::drawImage(query_point, &asciiPred);
		printf("Guessed %d for %d\n", prediction, query_point->getLabel());
		if (prediction == query_point->getLabel()) {
			count++;
		}
		else {
			//printf("Mistaked %d for %d\n", query_point->getLabel(), prediction);
		}
		data_index++;
		
		printf("Current Performance: %.3f %%\n", ((double)count*100.0) / ((double)data_index));
	}

	current_performance = ((double)count*100.0) / ((double)validation_data->size());
	printf("Validation Performance for K = %d: %.3f %%\n", k, current_performance);
	return current_performance;
}

double knn::test_performance() {
	double current_performance = 0;
	int count = 0;
	//int data_index = 0;

	for (data * query_point : * test_data) {
		find_knearest(query_point);
		int prediction = predict();
		if (prediction == query_point->getLabel()) {
			count++;
		}
		//data_index++;
		//printf("Current Performance: %.3f %%\n", ((double)count*100.0) / ((double)data_index));
	}

	current_performance = ((double)count*100.0) / ((double)test_data->size());
	printf("Test Performance: %.3f %%\n", current_performance);
	return current_performance;
}

/*int main() {
	data_handler *dh = new data_handler();
	//dh->read_feature_vector("C:/Users/stein/Desktop/Research Project 2020/Neural Networks/MNIST/Training Data/Training Data/train-images.idx3-ubyte");
	dh->read_feature_vector("../Training Data/train-images.idx3-ubyte");
	//dh->read_feature_labels("C:/Users/stein/Desktop/Research Project 2020/Neural Networks/MNIST/Training Data/Training Data/train-labels.idx1-ubyte");
	dh->read_feature_labels("../Training Data/train-labels.idx1-ubyte");
	dh->split_data();
	dh->count_classes();

	knn * knearest = new knn();
	knearest->set_training_Data(dh->get_training_data());
	knearest->set_test_Data(dh->get_test_data());
	knearest->set_validation_Data(dh->get_validation_data());

	double performance = 0;
	double best_performance = 0;
	int best_k = 1;
	
	for (int k = 1; k <= 20; k++) {
		if (k == 1) {
			knearest->set_k(k);
			performance = knearest->validate_performance();
			best_performance = performance;
		}
		else {
			knearest->set_k(k);
			performance = knearest->validate_performance();
			if (performance > best_performance) {
				best_performance = performance;
				best_k = k;
			}
		}
	}

	printf("Best K: %d", best_k);

	knearest->set_k(best_k);
	knearest->test_performance();

	printf("Closing...");
}*/