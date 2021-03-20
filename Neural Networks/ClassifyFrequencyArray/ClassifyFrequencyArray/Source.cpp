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


Network * mainNetwork(data_handler *dh)
{
	uint32_t size = dh->get_training_data()->at(0)->getNormalizedFeatureVector()->size();
	uint32_t class_count = dh->get_class_count();
	
	
	/// IMPORT
	FILE * f = fopen("Network_Output.net", "rb");
	unsigned long long file_size = 0;
	fread(&file_size, sizeof(file_size), 1, f);
	fseek(f, 0, SEEK_SET);
	char * buffer_read = (char*)calloc(file_size, sizeof(char));

	uint32_t red = fread(buffer_read, sizeof(char), file_size, f);

	fclose(f);

	printf("Buffer size: %llu.\t\tRead: %u\n", file_size, red);

	Network * net = new Network();
	net->Import(buffer_read);
	free(buffer_read);
	
	//std::vector<int> spec = { 8000, 8000, 4000 };
	//Network * net = new Network(spec, size, class_count);
	// lamba = [&](int target) {
	/*for (int target = 0; target < class_count; target++){
		net->target = target;
		net->set_training_Data(dh->get_training_data());
		net->set_test_Data(dh->get_test_data());
		net->set_validation_Data(dh->get_validation_data());
		printf("Size of net %d: %zu\n", target, sizeof(*net));
		for (int i = 0; i < 100; i++)
		{
			net->train_fast();
			if (i % 10 == 0)
				net->validate_fast();
		}
		net->test_fast();
		fprintf(stderr, "Test Performance for %d: %f -> Network Size: %zu\n", target, net->testPerformance, sizeof(*net));
	};*/
	net->set_training_Data(dh->get_training_data());
	net->set_test_Data(dh->get_test_data());
	net->set_validation_Data(dh->get_validation_data());

	//printf("Size of net %d: %zu\n", target, sizeof(*net));
	
	net->validationPerformance = 0;
	net->validate_fast();

	for (int i = 0; i < 100; i++)
	{
		net->train_fast();
		if (i % 10 == 0) {
			net->validate_fast();

			char * buffer;
			net->Export(&buffer);
			free(buffer);
		}
		data_handler::print_loading(false, i, 100);
	}
	net->test_fast();
	fprintf(stderr, "Test Performance: %f -> Network Size: %zu\n", net->testPerformance, sizeof(*net));

	/*std::vector<std::thread> threads;

	for (int i = 0; i < class_count; i++)
	{
		threads.emplace_back(std::thread(lamba, i));
	}

	for (auto &th : threads)
	{
		th.join();
	}*/

	return net;

	//return EXIT_SUCCESS;
}

int mainNetwork_Predict(data_handler *dh, Network * net) {
	net->set_training_Data(dh->get_training_data());
	net->set_test_Data(dh->get_test_data());
	net->set_validation_Data(dh->get_validation_data());

	int prediction = net->real_predict();

	printf("Final prediction: %d\n", prediction);

	return prediction;
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
	else if (argc == 5) {
		for (int i = 0; i < argc; i++) {
			printf("%d | %s\n", i, argv[i]);
		}

		char fn_vectors[128];
		char fn_labels[128];
		sprintf_s(fn_vectors, 128, "%s/%s.vectors", argv[1], argv[2]);
		sprintf_s(fn_labels, 128, "%s/%s.labels", argv[1], argv[2]);

		printf("Vectors: %s\nLabels: %s\n\n", fn_vectors, fn_labels);

		/*Network * net;
		if (true) {
			data_handler * dh_tmp = new data_handler();
			dh_tmp->read_feature_vector(fn_vectors);
			dh_tmp->read_feature_labels(fn_labels);
			dh_tmp->count_classes();
			dh_tmp->normalize();
			dh_tmp->split_data();
			net = mainNetwork(dh_tmp);

			delete dh_tmp;
		}
		else {
			FILE * f = fopen("network.net", "rb");
			fseek(f, 0, SEEK_END);
			uint64_t size = ftell(f);
			fseek(f, 0, SEEK_SET);
			fread(net, 1, size, f);
			fclose(f);
		}*/
		//argv[1] == Folder - output path
		//argv[2] == Dataset name
		

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

		if (strcmp(argv[4], "fast") == 0) {
			dh->split_data_fast();
		}
		else {
			dh->split_data();
		}

		//return mainNetwork_Predict(dh, net); // Notice that this is unconventional use of exit codes to pass data
		return mainKNN_Predict(dh); // Notice that this is unconventional use of exit codes to pass data
	}

	return 0;
}

int PredictLiveData() {


	return 0;
}




class Student {
	private:
		char * Name;

	public:
		char error_no_name[9] = "No Name!";
		char * getName() {
			if (Name == NULL) { return error_no_name; } // Make sure Name isn't empty
			Name[0] = toupper(Name[0]); // Force capitalize first letter of name
			return Name;
		}

		void setName(int student_name_length, char * student_name) {
			if (Name != NULL) { 
				printf("Student already has a name!\n");
				return; 
			}
			else if (student_name_length < 2) {
				printf("This cannot be a legal name, it is too short!\n");
				return;
			}
			else if (student_name == NULL) {
				printf("No name was given!\n");
				return;
			}
			else {
				memcpy(Name, student_name, student_name_length);
				printf("Student's name was changed to: '%s'.\n", Name);
			}
			return;
		}
};

typedef struct shape {/* Data */};
typedef struct food {/* Data */ };
typedef struct place {/* Data */ };


class Animal {
	protected:
		int age;
		shape body;
		food diet;
		place habitat;

	public:
		void Walk() {};
		void Talk();
};
class Bird : public Animal {
	protected: 
		int wing_span;
	public:
		void Fly();
};
class Pengin : public Bird {
	public:
		void Swim();
		void Fly() = delete; // Penguins cannot fly
};
class Dog : public Animal {

};


double Sum(int num1, int num2) {
	return num1 + num2;
}
double Sum(double num1, double num2) {
	return num1 + num2;
}
double Sum(float num1, float num2) {
	return num1 + num2;
}
double Sum(int length_of_array, double * num_array) {
	double sum = 0;
	for (int i = 0; i < length_of_array; i++) {
		sum += num_array[i];
	}
	return sum;
}


int mainTest() {

Animal * animals[4];

Animal * generic_animal = new Animal();
Bird * bird = new Bird();
Pengin * penguin = new Pengin();
Dog * dog = new Dog();

animals[0] = generic_animal;
animals[1] = bird;
animals[2] = penguin;
animals[3] = dog;

for (int i = 0; i < 4; i++) {
	animals[i]->Walk();
}
return 0;
}