#include "kmeans.h"

template<class T>
kmeans<T>::kmeans() {}
template<class T>
kmeans<T>::~kmeans() {}
template<class T>
kmeans<T>::kmeans(int k) {
	num_clusters = k;
	clusters = new std::vector<cluster_t<T> *>;
	used_indexes = new std::unordered_set<int>;
}
template<class T>
void kmeans<T>::init_clusters() {
	for (int i = 0; i < num_clusters; i++) {
		int index = (rand() % training_data->size());
		while (used_indexes->find(index) != used_indexes->end()) {
			index = (rand() % training_data->size());
		}
		clusters->push_back(new cluster_t<T>(training_data->at(index)));
		used_indexes->insert(index);
	}
}
template<class T>
void kmeans<T>::init_clusters_for_each_class() {
	std::unordered_set<int> classes_used;
	for (int i = 0; i < training_data->size(); i++) {
		if (classes_used.find(training_data->at(i)->get_label()) == classes_used.end()) {
			clusters->push_back(new cluster_t<T>(training_data->at(i)));
			classes_used.insert(training_data->at(i)->get_label());
			used_indexes->insert(i);
		}
	}
}
template<class T>
void kmeans<T>::train() {
	while (used_indexes->size() < training_data->size()) {
		int index = (rand() % training_data->size());
		while (used_indexes->find(index) != used_indexes->end()) {
			index = (rand() % training_data->size());
		}

		T min_dist = std::numeric_limits<T>::max();
		int best_cluster = 0;
		for (int j = 0; j < clusters->size(); j++) {
			T current_dist = euclidean_distance(clusters->at(j)->centroid, training_data->at(index));
			if (current_dist < min_dist) {
				min_dist = current_dist;
				best_cluster = j;
			}
		}
		clusters->at(best_cluster)->add_to_cluster(training_data->at(index));
		used_indexes->insert(index);
	}
}
template<class T>
T kmeans<T>::euclidean_distance(std::vector<T> * centroid, data<T> * point) {
	T dist = 0.0;
	for (int i = 0; i < centroid->size(); i++) {
		dist += pow(centroid->at(i) - point->get_feature_vector()->at(i), 2);
	}
	return sqrt(dist);
}
template<class T>
double kmeans<T>::validate() {
	double num_correct = 0.0;
	for (auto query_point : *validation_data) {
		T min_dist = std::numeric_limits<T>::max();
		int best_cluster = 0;
		for (int j = 0; j < clusters->size(); j++) {
			T current_dist = euclidean_distance(clusters->at(j)->centroid, query_point);
			if (current_dist < min_dist) {
				min_dist = current_dist;
				best_cluster = j;
			}
		}
		if (clusters->at(best_cluster)->most_frequent_class == query_point->get_label()) {
			num_correct++;
		}
	}

	return 100.0 * (num_correct / (double)validation_data->size());
}
template<class T>
double kmeans<T>::test() {
	double num_correct = 0.0;
	for (auto query_point : * test_data) {
		T min_dist = std::numeric_limits<T>::max();
		int best_cluster = 0;
		for (int j = 0; j < clusters->size(); j++) {
			T current_dist = euclidean_distance(clusters->at(j)->centroid, query_point);
			if (current_dist < min_dist) {
				min_dist = current_dist;
				best_cluster = j;
			}
		}
		if (clusters->at(best_cluster)->most_frequent_class == query_point->get_label()) {
			num_correct++;
		}
	}

	return 100.0 * (num_correct / (double)test_data->size());
}

