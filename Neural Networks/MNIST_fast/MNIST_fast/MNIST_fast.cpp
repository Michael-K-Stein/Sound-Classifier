#include <iostream>

#include "network.h"

#include <chrono>
#include <utility>
typedef std::chrono::high_resolution_clock::time_point TimeVar;

#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()

int main()
{
	srand(time(NULL));

	data_handler * _data_handler = new data_handler();
	_data_handler->read_input_data_file("C:\\Users\\stein\\Desktop\\Research Project 2020\\Neural Networks\\MNIST\\Training Data\\train-images.idx3-ubyte");
	_data_handler->read_input_label_file("C:\\Users\\stein\\Desktop\\Research Project 2020\\Neural Networks\\MNIST\\Training Data\\train-labels.idx1-ubyte");
	_data_handler->count_classes();
	_data_handler->split_data();

	std::vector<int> hidden_layers = { 32 };
	network * netw = new network(
		hidden_layers,
		_data_handler->get_training_data()->at(0)->get_feature_vector_size(),
		_data_handler->get_class_count(),
		0.25);

	netw->set_training_data(_data_handler->get_training_data());
	netw->set_validation_data(_data_handler->get_validation_data());
	netw->set_test_data(_data_handler->get_test_data());


	printf("Initial Validation: \n");
	printf("Validation performance: %.4f%%\n", 100.0 * netw->validate());

	netw->c_only();

	printf("Initial Validation - C: \n");
	printf("Validation performance: %.4f%%\n", 100.0 * netw->validate_c());
	printf("Now training...\n");

	

	for (int i = 0; i < 100000; i++) {

		//TimeVar t1 = timeNow();
		printf("Training error @ iteration %d: %.4f\n", i, netw->train_c());
		//double train_time = duration(timeNow() - t1);

		//printf("Training time: %.2f nanoseconds\n", train_time);
		
		if (i % 5 == 0) {
			//TimeVar t2 = timeNow();
			printf("Validation performance: %.4f%%\n", 100.0 * netw->validate_c());
			//double validate_time = duration(timeNow() - t2);
			//printf("Validating time: %.2f nanoseconds\n", validate_time);
		}


		if (i % 25 == 0) {
			printf("Test Performance: %.4f%%\n", 100.0 * netw->test_c());
		}
	}
	printf("Test Performance: %.4f%%\n", 100.0 * netw->test_c());

	return 0;
}