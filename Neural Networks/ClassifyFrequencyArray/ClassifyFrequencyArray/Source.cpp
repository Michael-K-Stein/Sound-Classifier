#include "knn.h"
#include "kmeans.h"
#include "network.h"

int mainKNN(data_handler *dh) {
	knn * knearest = new knn();
	knearest->set_training_Data(dh->get_training_data());
	knearest->set_test_Data(dh->get_test_data());
	knearest->set_validation_Data(dh->get_validation_data());

	double performance = 0;
	double best_performance = 0;
	int best_k = 1;

	for (int k = 1; k <= 1; k++) {
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

	printf("Best K: %d\n", best_k);

	knearest->set_k(best_k);
	knearest->test_performance();

	printf("Closing...");



	return 0;
}


int mainKMEANS(data_handler * dh) {
	//data_handler *dh = new data_handler();
	////dh->read_feature_vector("C:/Users/stein/Desktop/Research Project 2020/Neural Networks/MNIST/Training Data/Training Data/train-images.idx3-ubyte");
	//dh->read_feature_vector("../Training Data/train-images.idx3-ubyte");
	////dh->read_feature_labels("C:/Users/stein/Desktop/Research Project 2020/Neural Networks/MNIST/Training Data/Training Data/train-labels.idx1-ubyte");
	//dh->read_feature_labels("../Training Data/train-labels.idx1-ubyte");
	//dh->split_data();
	//dh->count_classes();



	double performance = 0;
	double best_performance = 0;
	int best_k = 1;

	for (int k = dh->get_class_count(); k < dh->get_training_data()->size() * 0.1; k++) {

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


int mainNetwork(data_handler *dh)
{
	uint32_t size = dh->get_training_data()->at(0)->getNormalizedFeatureVector()->size();
	uint32_t class_count = dh->get_class_count();
	std::vector<int> spec = { 2 };
	auto lamba = [&](int target) {
		Network * net = new Network(spec, size, class_count);
		net->target = target;
		net->set_training_Data(dh->get_training_data());
		net->set_test_Data(dh->get_test_data());
		net->set_validation_Data(dh->get_validation_data());
		printf("Size of net %d: %zu\n", target, sizeof(*net));
		for (int i = 0; i < 100; i++)
		{
			net->train();
			if (i % 10 == 0)
				net->validate();
		}
		net->test();
		fprintf(stderr, "Test Performance for %d: %f -> Network Size: %zu\n", target, net->testPerformance, sizeof(*net));
	};

	std::vector<std::thread> threads;

	for (int i = 0; i < class_count; i++)
	{
		threads.emplace_back(std::thread(lamba, i));
	}

	for (auto &th : threads)
	{
		th.join();
	}

	return EXIT_SUCCESS;
}


int mainKNN_Predict(data_handler *dh) {
	knn * knearest = new knn();
	knearest->set_training_Data(dh->get_training_data());
	knearest->set_test_Data(dh->get_test_data());
	knearest->set_validation_Data(dh->get_validation_data());

	knearest->set_k(1);
	int prediction = knearest->real_predict();

	printf("Final prediction: %d\n", prediction);

	return prediction;
}

int main(int argc, char ** argv) {
	if (argc == 1) {
		/// Generic Learning
		data_handler * dh = new data_handler();
		//dh->read_feature_vector("C:/Users/stein/Desktop/Research Project 2020/Neural Networks/MNIST/Training Data/Training Data/train-images.idx3-ubyte");
		//dh->read_feature_vector("../Training Data/train-images.idx3-ubyte");
		dh->read_feature_vector("../Training Data/TestOne.vectors");
		//dh->read_feature_labels("C:/Users/stein/Desktop/Research Project 2020/Neural Networks/MNIST/Training Data/Training Data/train-labels.idx1-ubyte");
		dh->read_feature_labels("../Training Data/TestOne.labels");
		dh->count_classes();
		dh->normalize();
		dh->split_data();

		mainKNN(dh);
		//mainNetwork(dh);
	}
	else if (argc == 3) {
		/// Predict from file
		// argv[1] == vector file
		// argv[2] == label file

		/// Load known data
		data_handler * dh = new data_handler();
		//dh->read_feature_vector("C:/Users/stein/Desktop/Research Project 2020/Neural Networks/MNIST/Training Data/Training Data/train-images.idx3-ubyte");
		//dh->read_feature_vector("../Training Data/train-images.idx3-ubyte");
		dh->read_feature_vector("../Training Data/TestOne.vectors");
		//dh->read_feature_labels("C:/Users/stein/Desktop/Research Project 2020/Neural Networks/MNIST/Training Data/Training Data/train-labels.idx1-ubyte");
		dh->read_feature_labels("../Training Data/TestOne.labels");
		dh->read_predict_feature_vector(argv[1]);
		dh->read_predict_feature_labels(argv[2]);
		dh->count_classes();
		dh->normalize();
		dh->split_data();

		mainKNN(dh);
	}
	else if (argc == 4) {
		for (int i = 0; i < argc; i++) {
			printf("%d | %s\n", i, argv[i]);
		}

		//argv[1] == Folder - output path
		//argv[2] == Dataset name
		char fn_vectors[128];
		char fn_labels[128];
		sprintf_s(fn_vectors, 128, "%s/%s.vectors", argv[1], argv[2]);
		sprintf_s(fn_labels, 128, "%s/%s.labels", argv[1], argv[2]);

		printf("Vectors: %s\nLabels: %s\n\n", fn_vectors, fn_labels);

		/// Predict from file
		// argv[3] == vector file

		/// Load known data
		data_handler * dh = new data_handler();
		dh->read_feature_vector(fn_vectors);
		dh->read_feature_labels(fn_labels);
		/// Load prediction vectors
		dh->read_predict_feature_vector(argv[3]);
		dh->count_classes();
		dh->normalize();
		dh->split_data();

		return mainKNN_Predict(dh); // Notice that this is unconventional use of exit codes to pass data
	}

	return 0;
}

int PredictLiveData() {


	return 0;
}