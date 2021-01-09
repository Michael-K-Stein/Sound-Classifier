#include "data_handler.h"

uint8_t print_loading_prev_val = 0;
void data_handler::print_loading(bool print_dots, int amt, int cap) {
	if (print_dots) {
		printf("\rLoading");
		for (int i = 0; i < print_loading_prev_val; i++) {
			printf(".");
		}
		print_loading_prev_val = (print_loading_prev_val + 1) % 3;
	}
	else {
		printf("\rLoading %.2f", (float)amt * 100.0 / (float)cap);
	}
}

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

void data_handler::read_csv(std::string path, std::string delimiter) {
	num_classes = 0;
	std::ifstream data_file(path.c_str());
	std::string line;
	while (std::getline(data_file, line)) {
		if (line.length() == 0) { continue; }
		data * d = new data();
		d->set_double_feature_vector(new std::vector<double>());
		size_t position = 0;
		std::string token; // value in between delimiter
		while ((position = line.find(delimiter)) != std::string::npos) {
			token = line.substr(0, position);
			d->append_to_feature_vector(std::stod(token));
			line.erase(0, position + delimiter.length());
		}
		if (classMap.find(line) != classMap.end()) {
			d->set_label(classMap[line]);
		}
		else {
			classMap[line] = num_classes;
			d->set_label(classMap[line]);
			num_classes++;
		}
		data_array->push_back(d);
	}
	feature_vector_size = data_array->at(0)->get_double_feature_vector()->size();
}

void data_handler::read_feature_vector(std::string filePath) {
	uint32_t		header[3]; // Magic | Num Samples | Sample Size
	unsigned char	bytes[4];
	FILE *f = fopen(filePath.c_str(), "rb");
	if (f) {
		for (int i = 0; i < 3; i++) {
			if (fread(bytes, sizeof(bytes), 1, f)) {
				header[i] = convert_to_little_endian(bytes);
			}
		}
		printf("Done getting input file header.\n");
		uint32_t sample_size = header[2];
		
		uint32_t sample_count = std::min(header[1], max_image_array);

		for (int i = 0; i < sample_count; i++)
		{
			data *d = new data();
			d->set_feature_vector(new std::vector<uint8_t>());
			uint8_t * element = (uint8_t *)calloc(sample_size, sizeof(uint8_t));
			fread(element, 1, sample_size, f);
			for (int j = 0; j < sample_size; j++)
			{
				if (true) {
					d->append_to_feature_vector(element[j]);
				}
				else {
					printf("Error reading from file.\n");
				}
			}
			free(element);
#ifdef FAST_DISTANCE
			d->create_feature_array();
#endif
			data_array->push_back(d);
		}

		//for (int i = 0; i < header[1]; i++) {
		//	data *d = new data();
		//	uint8_t element[1];
		//	//fread(element, 1, image_size, f);
		//	for (int j = 0; j < image_size; j++) {
		//		d->append_to_feature_vector(element[0]);
		//	}
		//	data_array->push_back(d);
		//}


		//normalize();

		printf("Successfully read and stored %lu feature vectors.\n", data_array->size());
	} else {
		printf("Could not find file.\n");
	}
}

void data_handler::read_feature_labels(std::string filePath) {
	uint32_t		header[2]; // Magic | Num Samples
	unsigned char	bytes[4];
	FILE *f = fopen(filePath.c_str(), "rb");
	if (f) {
		for (int i = 0; i < 2; i++) {
			if (fread(bytes, sizeof(bytes), 1, f)) {
				header[i] = convert_to_little_endian(bytes);
			}
		}
		printf("Done getting label file header.\n");

		uint32_t sample_count = std::min(header[1], max_image_array);

		for (int i = 0; i < sample_count; i++) {
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

void data_handler::read_predict_feature_vector(std::string filePath) {
	original_data_size = data_array->size();

	uint32_t		header[3]; // Magic | Num Samples | Sample Size
	unsigned char	bytes[4];
	FILE *f = fopen(filePath.c_str(), "rb");
	if (f) {
		for (int i = 0; i < 3; i++) {
			if (fread(bytes, sizeof(bytes), 1, f)) {
				header[i] = convert_to_little_endian(bytes);
			}
		}
		printf("Done getting predict file header.\n");
		uint32_t sample_size = header[2];

		uint32_t sample_count = std::min(header[1], max_image_array);

		for (int i = 0; i < sample_count; i++)
		{
			data *d = new data();
			d->set_feature_vector(new std::vector<uint8_t>());
			uint8_t * element = (uint8_t *)calloc(sample_size, sizeof(uint8_t));
			fread(element, 1, sample_size, f);
			for (int j = 0; j < sample_size; j++)
			{
				if (true) {
					d->append_to_feature_vector(element[j]);
				}
				else {
					printf("Error reading from predict file.\n");
				}
			}
			free(element);

#ifdef FAST_DISTANCE
			d->create_feature_array();
#endif
			data_array->push_back(d);
		}

		//for (int i = 0; i < header[1]; i++) {
		//	data *d = new data();
		//	uint8_t element[1];
		//	//fread(element, 1, image_size, f);
		//	for (int j = 0; j < image_size; j++) {
		//		d->append_to_feature_vector(element[0]);
		//	}
		//	data_array->push_back(d);
		//}


		//normalize();

		predict_data_size = data_array->size() - original_data_size;
		printf("Successfully read and stored %lu feature vectors from predict file.\n", predict_data_size);
	}
	else {
		printf("Could not find predict file.\n");
	}
}

void data_handler::read_predict_feature_labels(std::string filePath) {
	uint32_t		header[2]; // Magic | Num Samples
	unsigned char	bytes[4];
	FILE *f = fopen(filePath.c_str(), "rb");
	if (f) {
		for (int i = 0; i < 2; i++) {
			if (fread(bytes, sizeof(bytes), 1, f)) {
				header[i] = convert_to_little_endian(bytes);
			}
		}
		printf("Done getting label file header.\n");

		uint32_t sample_count = std::min(header[1], max_image_array);

		for (int i = original_data_size; i < sample_count + original_data_size; i++) {
			data *d = new data();
			uint8_t element[1];

			if (fread(element, sizeof(element), 1, f)) {
				data_array->at(i)->set_label(element[0]);
			}
			else {
				printf("Error reading from predict label file.\n");
				exit(1);
			}
		}
		printf("Successfully read and stored predict label vectors.\n");
	}
	else {
		printf("Could not find predict label file.\n");
	}
}

void data_handler::normalize()
{
	std::vector<double> mins, maxs;
	// fill min and max lists

	data *d = data_array->at(0);
	for (auto val : *d->get_feature_vector())
	{
		mins.push_back(val);
		maxs.push_back(val);
	}

	for (int i = 1; i < data_array->size(); i++)
	{
		print_loading(false, i, data_array->size());
		d = data_array->at(i);
		for (int j = 0; j < d->get_feature_vector_size(); j++)
		{
			double value = (double)d->get_feature_vector()->at(j);
			if (value < mins.at(j)) mins[j] = value;
			if (value > maxs.at(j)) maxs[j] = value;
		}
	}
	// normalize data array
	printf("\r                \r");
	for (int i = 0; i < data_array->size(); i++)
	{
		print_loading(false, i, data_array->size());
		data_array->at(i)->setNormalizedFeatureVector(new std::vector<double>());
		data_array->at(i)->set_class_vector(get_class_count());
		for (int j = 0; j < data_array->at(i)->get_feature_vector_size(); j++)
		{
			if (maxs[j] - mins[j] == 0) data_array->at(i)->append_to_feature_vector(0.0);
			else
				data_array->at(i)->append_to_feature_vector((double)(data_array->at(i)->get_feature_vector()->at(j) - mins[j]) / (maxs[j] - mins[j]));
		}
	}
	printf("\r                \r");
}


void data_handler::split_data() {
	std::unordered_set <int> used_indexes;
	if (predict_data_size == 0) {
		int train_size = data_array->size() * TRAIN_SET_PERCENT;
		int test_size = data_array->size() * TEST_SET_PERCENT;
		int valid_size = data_array->size() * VALIDATION_PERCENT;

		// Training Data

		uint32_t count = 0;
		while (count < train_size) {
			print_loading(true, 0, 1);
			//uint32_t rand_index = count;
			uint32_t rand_index = rand() % data_array->size();  // Takes too long
			if (used_indexes.find(rand_index) == used_indexes.end()) {
				training_data->push_back(data_array->at(rand_index));
				used_indexes.insert(rand_index);
				count++;
			}
		}

		// Test Data

		count = 0;
		while (count < test_size) {
			print_loading(true, 0, 1);
			uint32_t rand_index = rand() % data_array->size();  // Takes too long
			//uint32_t rand_index = train_size + count;
			if (used_indexes.find(rand_index) == used_indexes.end()) {
				test_data->push_back(data_array->at(rand_index));
				used_indexes.insert(rand_index);
				count++;
			}
		}

		// Validation Data

		count = 0;
		while (count < valid_size) {
			print_loading(true, 0, 1);
			uint32_t rand_index = rand() % data_array->size();  // Takes too long
			//uint32_t rand_index = train_size + test_size + count;
			if (used_indexes.find(rand_index) == used_indexes.end()) {
				validation_data->push_back(data_array->at(rand_index));
				used_indexes.insert(rand_index);
				count++;
			}
		}
	}
	else {
		
		uint32_t count = 0;
		while (count < original_data_size) {
			print_loading(true, 0, 1);
			uint32_t rand_index = rand() % original_data_size;  // Takes too long
			if (used_indexes.find(rand_index) == used_indexes.end()) {
				training_data->push_back(data_array->at(rand_index));
				used_indexes.insert(rand_index);
				count++;
			}
		}

		count = 0;
		while (count < predict_data_size) {
			print_loading(true, 0, 1);
			uint32_t rand_index = (rand() % predict_data_size) + original_data_size;  // Takes too long
			if (used_indexes.find(rand_index) == used_indexes.end()) {
				test_data->push_back(data_array->at(rand_index));
				used_indexes.insert(rand_index);
				count++;
			}
		}
	}
	printf("\r                \r");
	printf("Training Data Size: %lu.\n", training_data->size());
	printf("Test Data Size: %lu.\n", test_data->size());
	printf("Validation Data Size: %lu.\n", validation_data->size());
}
void data_handler::split_data_fast() {
	std::unordered_set <int> used_indexes;
	uint32_t count = 0;
	while (count < original_data_size) {
		print_loading(true, 0, 1);
		uint32_t rand_index = rand() % original_data_size;  // Takes too long
		if (used_indexes.find(rand_index) == used_indexes.end()) {
			training_data->push_back(data_array->at(rand_index));
			used_indexes.insert(rand_index);
			count++;
		}
	}

	count = 0;
	while (count < (int)(predict_data_size / 10)) {
		print_loading(true, 0, 1);
		uint32_t rand_index = (rand() % predict_data_size) + original_data_size;  // Takes too long
		if (used_indexes.find(rand_index) == used_indexes.end()) {
			test_data->push_back(data_array->at(rand_index));
			used_indexes.insert(rand_index);
			count++;
		}
	}
	printf("\r                \r");
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

uint32_t data_handler::convert_to_little_endian(const unsigned char * bytes) {
	return (uint32_t)((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3]));
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

int data_handler::get_class_count() {
	return num_classes;
}