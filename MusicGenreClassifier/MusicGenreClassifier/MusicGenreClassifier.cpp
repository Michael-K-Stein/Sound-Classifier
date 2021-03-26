#include "CommandLineFunctions.h"

#include <chrono>
#include <utility>
typedef std::chrono::high_resolution_clock::time_point TimeVar;

#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()

int StartUp() {
	std::cout << "=== === === === ===" << std::endl;
	std::cout << "Started WAV Analyzer" << std::endl;
	std::cout << "  Data Set Editor" << std::endl;
	std::cout << "=== === === === ===" << std::endl;
	std::cout << std::endl << std::endl;

	ChangeOutputPath();
	ChangeDataSetName();

	while (true) {
		if (ProccessCommands() == 1) {
			printf("# Command Failed!\n");
		}
	}

	return 0;
}

int main()
{
	StartUp();

	/*srand(time(NULL));

	Data_Handler * _data_handler = new Data_Handler();
	_data_handler->read_input_data_file("C:\\Users\\stein\\Desktop\\Research Project 2020\\Neural Networks\\MNIST\\Training Data\\train-images.idx3-ubyte");
	_data_handler->read_input_label_file("C:\\Users\\stein\\Desktop\\Research Project 2020\\Neural Networks\\MNIST\\Training Data\\train-labels.idx1-ubyte");
	_data_handler->count_classes();
	_data_handler->split_data();

	Network * netw;

	if (1) { 
		/// IMPORT
		FILE * f;
		errno_t err = fopen_s(&f, "Network_Output.net", "rb");
		unsigned long long file_size = 0;
		fread(&file_size, sizeof(file_size), 1, f);
		fseek(f, 0, SEEK_SET);
		char * buffer_read = (char*)calloc(file_size, sizeof(char));

		uint32_t red = fread(buffer_read, sizeof(char), file_size, f);

		fclose(f);

		printf("Buffer size: %llu.\t\tRead: %u\n", file_size, red);

		netw = new Network(0.25);
		netw->import_network(buffer_read);
		free(buffer_read);
		_data_handler->c_only();

		netw->set_training_data(_data_handler->get_training_data());
		netw->set_validation_data(_data_handler->get_validation_data());
		netw->set_test_data(_data_handler->get_test_data());
	}
	else {
		std::vector<int> hidden_layers = { 32 };
		netw = new Network(
			hidden_layers,
			_data_handler->get_training_data()->at(0)->get_feature_vector_size(),
			_data_handler->get_class_count(),
			0.25);

		netw->set_training_data(_data_handler->get_training_data());
		netw->set_validation_data(_data_handler->get_validation_data());
		netw->set_test_data(_data_handler->get_test_data());

		netw->c_only();
	}


	printf("Initial Test - C: \n");
	printf("Test performance: %.4f%%\n", 100.0 * netw->test_c());
	printf("Now training...\n");

	double old_perf = 0;

	for (int i = 0; i < 100000; i++) {

		//TimeVar t1 = timeNow();
		printf("Training error @ iteration %d: %.4f\n", i, netw->train_c());
		//double train_time = duration(timeNow() - t1);

		//printf("Training time: %.2f nanoseconds\n", train_time);

		if (i % 5 == 0) {
			//TimeVar t2 = timeNow();
			printf("Validation performance: %.4f%%\n", 100.0 * netw->validate_c());
			
			if (netw->test_performance > old_perf) {
				char * buffer;
				netw->export_network(&buffer);
				free(buffer);
				old_perf = netw->test_performance;
			}

			//double validate_time = duration(timeNow() - t2);
			//printf("Validating time: %.2f nanoseconds\n", validate_time);
		}


		if (i % 25 == 0) {
			printf("Test Performance: %.4f%%\n", 100.0 * netw->test_c());
		}
	}
	printf("Test Performance: %.4f%%\n", 100.0 * netw->test_c());*/

	return 0;
}