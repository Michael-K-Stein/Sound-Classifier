#include "knn.h"
#include "kmeans.h"
#include "network.h"

int mainKNN() {
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
	return 0;
}

int mainKMEANS() {
	data_handler *dh = new data_handler();
	//dh->read_feature_vector("C:/Users/stein/Desktop/Research Project 2020/Neural Networks/MNIST/Training Data/Training Data/train-images.idx3-ubyte");
	dh->read_feature_vector("../Training Data/train-images.idx3-ubyte");
	//dh->read_feature_labels("C:/Users/stein/Desktop/Research Project 2020/Neural Networks/MNIST/Training Data/Training Data/train-labels.idx1-ubyte");
	dh->read_feature_labels("../Training Data/train-labels.idx1-ubyte");
	dh->split_data();
	dh->count_classes();





	double performance = 0;
	double best_performance = 0;
	int best_k = 1;

	for (int k = /*dh->get_class_count()*/462; k < 463 /*dh->get_training_data()->size() * 0.1*/; k++) {

		kmeans * kMeans = new kmeans(k);
		kMeans->set_training_Data(dh->get_training_data());
		kMeans->set_test_Data(dh->get_test_data());
		kMeans->set_validation_Data(dh->get_validation_data());
		kMeans->init_clusters();
		kMeans->train();
		performance = kMeans->validate();
		printf("Current performance at K = %d: %.2f\n", k, performance);
		if (performance > best_performance) {
			best_performance = performance;
			best_k = k;
		}
	}

	kmeans * kMeans = new kmeans(best_k);
	kMeans->set_training_Data(dh->get_training_data());
	kMeans->set_test_Data(dh->get_test_data());
	kMeans->set_validation_Data(dh->get_validation_data());
	kMeans->init_clusters();
	performance = kMeans->test();
	printf("Best (test) performance at K = %d: %.2f\n", best_k, performance);

	printf("Closing...");

	return 0;
}

int mainNetwork()
{
	data_handler *dh = new data_handler();
	//#ifdef MNIST
	dh->read_feature_vector("../Training Data/train-images.idx3-ubyte");
	dh->read_feature_labels("../Training Data/train-labels.idx1-ubyte");
	dh->count_classes();

	dh->normalize();
	//#else
	//    dh->readCsv("../Data/iris.data", ",");
	//#endif
	dh->split_data();
	std::vector<int> hiddenLayers = { 10 };
	auto lamba = [&](int th) {
		Network * net = new Network(hiddenLayers, dh->get_training_data()->at(0)->getNormalizedFeatureVector()->size(), dh->get_class_count(), 0.05, th);
		//net->target = target;
		net->set_training_Data(dh->get_training_data());
		net->set_test_Data(dh->get_test_data());
		net->set_validation_Data(dh->get_validation_data());
		//printf("Size of net %d: %zu\n", target, sizeof(*net));
		//for (int i = 0; i < 3; i++)
		//{
			net->train(15);
		//	if (i % 3 == 0)
			net->validate();
		//}
		net->test();
		fprintf(stderr, "Test Performance for thread #%d: %f\n", th, net->testPerformance);
	};

	std::vector<std::thread> threads;

	for (int i = 0; i < 2; i++)
	{
		threads.emplace_back(std::thread(lamba, i));
	}

	for (auto &th : threads)
	{
		th.join();
	}

	return EXIT_SUCCESS;
}


int main() {

	//mainKMEANS();

	//mainKNN();

	mainNetwork();

	return 0;
}