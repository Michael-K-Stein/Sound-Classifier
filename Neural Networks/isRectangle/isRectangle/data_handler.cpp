#include "data_handler.h"



data_handler::data_handler()
{
	data_array = new std::vector<data *>;
	test_data = new std::vector<data *>;
	training_data = new std::vector<data *>;
	validation_data = new std::vector<data *>;
}


data_handler::~data_handler()
{
}

uint32_t data_handler::convert_to_uint32(const unsigned char * bytes) {
	return (uint32_t)bytes[0];
	//return (uint32_t)((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3]));
}

void data_handler::read_feature_vector(std::string filePath) {
	uint32_t		header[3]; // Num Images | Row Size | Col Size 
	unsigned char	byte[1];
	FILE *f = fopen(filePath.c_str(), "rb");

	if (f) {
		for (int i = 0; i < 3; i++) {
			
			fread(byte, sizeof(byte), 1, f);
			header[i] = convert_to_uint32(byte);
		}

		printf("Done getting input file header.\n");
		uint32_t image_size = header[1] * header[2];

		uint32_t image_count = std::min(header[0], max_image_array);

		for (int i = 0; i < image_count; i++)
		{
			data *d = new data();
			d->set_feature_vector(new std::vector<uint8_t>());
			uint8_t element[1024];
			fread(element, 1, image_size, f);
			for (int j = 0; j < image_size; j++)
			{
				if (true) {
					d->append_to_feature_vector(element[j]);
				}
				else {
					printf("Error reading from file.\n");
				}
			}

			//Graphic::drawImage(d);

			data_array->push_back(d);
		}
		printf("Successfully read and stored %lu feature vectors.\n", data_array->size());
	}
	else {
		printf("Could not find file.\n");
	}
}

void data_handler::read_feature_labels(std::string filePath) {
	uint32_t		header[1]; // Num Images
	unsigned char	byte[1];
	FILE *f = fopen(filePath.c_str(), "rb");
	if (f) {
		fread(byte, sizeof(byte), 1, f);
		header[0] = convert_to_uint32(byte);
		printf("Done getting label file header.\n");

		uint32_t image_count = std::min(header[0], max_image_array);

		for (int i = 0; i < image_count; i++) {
			data *d = new data();
			uint8_t element[1];

			if (fread(element, sizeof(element), 1, f)) {
				data_array->at(i)->set_label(element[0]);
			}
			else {
				printf("Error reading from file.\n");
				exit(1);
			}
		}
		printf("Successfully read and stored label vectors.\n");
	}
	else {
		printf("Could not find file.\n");
	}
}

void data_handler::split_data() {
	std::unordered_set <int> used_indexes;
	int train_size = data_array->size() * TRAIN_SET_PERCENT;
	int test_size = data_array->size() * TEST_SET_PERCENT;
	int valid_size = data_array->size() * VALIDATION_PERCENT;

	// Training Data

	uint32_t count = 0;
	while (count < train_size) {
		uint32_t rand_index = count;
		//uint32_t rand_index = rand() % data_array->size();
		if (used_indexes.find(rand_index) == used_indexes.end()) {
			training_data->push_back(data_array->at(rand_index));
			used_indexes.insert(rand_index);
			count++;
		}
	}

	// Test Data

	count = 0;
	while (count < test_size) {
		//uint32_t rand_index = rand() % data_array->size();
		uint32_t rand_index = train_size + count;
		if (used_indexes.find(rand_index) == used_indexes.end()) {
			test_data->push_back(data_array->at(rand_index));
			used_indexes.insert(rand_index);
			count++;
		}
	}

	// Validation Data

	count = 0;
	while (count < valid_size) {
		//uint32_t rand_index = rand() % data_array->size();
		uint32_t rand_index = train_size + test_size + count;
		if (used_indexes.find(rand_index) == used_indexes.end()) {
			validation_data->push_back(data_array->at(rand_index));
			used_indexes.insert(rand_index);
			count++;
		}
	}

	printf("Training Data Size: %lu.\n", training_data->size());
	printf("Test Data Size: %lu.\n", test_data->size());
	printf("Validation Data Size: %lu.\n", validation_data->size());
}

void data_handler::count_classes() {
	int count = 0;
	for (unsigned i = 0; i < data_array->size(); i++) {
		if (class_map.find(data_array->at(i)->get_label()) == class_map.end()) {
			class_map[data_array->at(i)->get_label()] = count;
			data_array->at(i)->set_enumerated_label(count);
			count++;
		}
	}
	num_classes = count;
	printf("Extracted %d unique classes.\n", num_classes);
}

std::vector<data *> * data_handler::get_training_data() {
	return training_data;
}
std::vector<data *> * data_handler::get_test_data() {
	return test_data;
}
std::vector<data *> * data_handler::get_validation_data() {
	return validation_data;
}

