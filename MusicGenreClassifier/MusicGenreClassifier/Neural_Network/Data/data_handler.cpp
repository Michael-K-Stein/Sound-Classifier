#include "data_handler.h"

Data_Handler::Data_Handler() {
	data_vector = new std::vector<Data *>;
	training_data = new std::vector<Data *>;
	test_data = new std::vector<Data *>;
	validation_data = new std::vector<Data *>;
	prediction_data = new std::vector<Data *>;
}
Data_Handler::~Data_Handler() {
}

void Data_Handler::read_input_data_file(std::string file_path) {
	uint32_t magic = 0;
	uint32_t num_images = 0;
	uint32_t num_rows = 0;
	uint32_t num_cols = 0;

	unsigned char bytes[4];
	FILE *f;
	errno_t err = fopen_s(&f, file_path.c_str(), "rb");
	if (f)
	{
		int i = 0;
		while (i < 4)
		{
			if (fread(bytes, sizeof(bytes), 1, f))
			{
				switch (i)
				{
				case 0:
					magic = format(bytes);
					i++;
					break;
				case 1:
					num_images = format(bytes);
					i++;
					break;
				case 2:
					num_rows = format(bytes);
					i++;
					break;
				case 3:
					num_cols = format(bytes);
					i++;
					break;
				}
			}
		}
		num_images = std::min(num_images, max_data_vector_size);
		printf("Done getting input data file header.\n");

		uint32_t image_size = num_rows * num_cols;
		for (i = 0; i < num_images; i++)
		{
			Data * d = new Data();
			d->set_feature_vector(new std::vector<uint8_t>());
			uint8_t element[1];
			for (int j = 0; j < image_size; j++)
			{
				if (fread(element, sizeof(element), 1, f))
				{
					d->append_to_feature_vector(element[0]);
				}
			}
			this->data_vector->push_back(d);
			d->set_class_vector(class_counts);
		}
		normalize_data();
		feature_vector_size = data_vector->at(0)->get_feature_vector_size();

		printf("Successfully read %lu data entries.\n", data_vector->size());
		printf("The Feature Vector Size is: %d\n", feature_vector_size);
	}
	else
	{
		printf("Could not open or read input vectors file!\n");
		printf("Now exiting...\n");
		exit(1);
	}
}
void Data_Handler::read_input_label_file(std::string file_path) {
	uint32_t magic = 0;
	uint32_t num_images = 0;
	unsigned char bytes[4];
	FILE *f;
	errno_t err = fopen_s(&f, file_path.c_str(), "rb");
	if (f)
	{
		int i = 0;
		while (i < 2)
		{
			if (fread(bytes, sizeof(bytes), 1, f))
			{
				switch (i)
				{
				case 0:
					magic = format(bytes);
					i++;
					break;
				case 1:
					num_images = format(bytes);
					i++;
					break;
				}
			}
		}

		printf("Done getting Label header.\n");

		num_images = std::min(num_images, max_data_vector_size);

		for (unsigned j = 0; j < num_images; j++)
		{
			uint8_t element[1];
			if (fread(element, sizeof(element), 1, f))
			{
				data_vector->at(j)->set_label(element[0]);
			}
		}

		printf("Successfully labeled %lu data entries.\n", num_images);
	}
	else
	{
		printf("Could not open or read input labels file!\n");
		printf("Now exiting...\n");
		exit(1);
	}
}

void Data_Handler::read_feature_vector(std::string filePath) {
	uint32_t		header[3]; // Magic | Num Samples | Sample Size
	unsigned char	bytes[4];
	FILE *f;
	errno_t err = fopen_s(&f, filePath.c_str(), "rb");
	if (f) {
		for (int i = 0; i < 3; i++) {
			if (fread(bytes, sizeof(bytes), 1, f)) {
				header[i] = format(bytes);
			}
		}
		printf("Done getting input file header.\n");
		uint32_t sample_size = header[2];

		uint32_t sample_count = std::min(header[1], max_data_vector_size);

		for (int i = 0; i < sample_count; i++)
		{
			Data *d = new Data();
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
			data_vector->push_back(d);
		}

		printf("Successfully read and stored %lu feature vectors.\n", data_vector->size());
	}
	else {
		printf("Could not find file.\n");
	}
}

void Data_Handler::read_feature_labels(std::string filePath) {
	uint32_t		header[2]; // Magic | Num Samples
	unsigned char	bytes[4];
	FILE *f;
	errno_t err = fopen_s(&f, filePath.c_str(), "rb");
	if (f) {
		for (int i = 0; i < 2; i++) {
			if (fread(bytes, sizeof(bytes), 1, f)) {
				header[i] = format(bytes);
			}
		}
		printf("Done getting label file header.\n");

		uint32_t sample_count = std::min(header[1], max_data_vector_size);

		for (int i = 0; i < sample_count; i++) {
			Data *d = new Data();
			uint8_t element[1];

			if (fread(element, sizeof(element), 1, f)) {
				data_vector->at(i)->set_label(element[0]);
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

void Data_Handler::read_predict_feature_vector(std::string filePath) {
	prediction_data->clear();

	uint32_t		header[3]; // Magic | Num Samples | Sample Size
	unsigned char	bytes[4];
	FILE *f;
	errno_t err = fopen_s(&f, filePath.c_str(), "rb");
	if (f) {
		for (int i = 0; i < 3; i++) {
			if (fread(bytes, sizeof(bytes), 1, f)) {
				header[i] = format(bytes);
			}
		}
		printf("Done getting predict file header.\n");
		uint32_t sample_size = header[2];

		uint32_t sample_count = std::min(header[1], max_data_vector_size);

		for (int i = 0; i < sample_count; i++)
		{
			Data * d = new Data();
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

			prediction_data->push_back(d);
		}

		predict_data_size = prediction_data->size();
		printf("Successfully read and stored %lu feature vectors from predict file.\n", predict_data_size);
	}
	else {
		printf("Could not find predict file.\n");
	}
}

void Data_Handler::split_data() {

	training_data->clear();
	validation_data->clear();
	test_data->clear();

	std::unordered_set<int> used_indexes;
	int train_size = data_vector->size() * TRAIN_SET_PERCENT;
	int validation_size = data_vector->size() * VALID_SET_PERCENT;
	int test_size = data_vector->size() * TEST_SET_PERCENT;

	std::random_shuffle(data_vector->begin(), data_vector->end());

	// Training Data
	int count = 0;
	int index = 0;
	while (count < train_size)
	{
		training_data->push_back(data_vector->at(index++));
		count++;
	}

	// Validation Data
	count = 0;
	while (count < validation_size)
	{
		validation_data->push_back(data_vector->at(index++));
		count++;
	}

	// Test Data
	count = 0;
	while (count < test_size)
	{
		test_data->push_back(data_vector->at(index++));
		count++;
	}

	printf("Training Data Size: %lu.\n", training_data->size());
	printf("Validation Data Size: %lu.\n", validation_data->size());
	printf("Test Data Size: %lu.\n", test_data->size());

	if (training_data->size() + validation_data->size() + test_data->size() != data_vector->size()) {
		uint32_t amount_used = training_data->size() + validation_data->size() + test_data->size();
		printf("Only %.2f%% of vectors were used. %d were not sorted.\n", 100.0 * amount_used / data_vector->size(), data_vector->size() - amount_used);
	}
}

void Data_Handler::count_classes() {
	int count = 0;
	for (unsigned i = 0; i < data_vector->size(); i++)
	{
		if (class_from_int.find(data_vector->at(i)->get_label()) == class_from_int.end())
		{
			class_from_int[data_vector->at(i)->get_label()] = count;
			data_vector->at(i)->set_enumerated_label(count);
			count++;
		}
		else
		{
			data_vector->at(i)->set_enumerated_label(class_from_int[data_vector->at(i)->get_label()]);
		}
	}

	class_counts = count;
	for (Data * data : *data_vector) {
		data->set_class_vector(class_counts);
		//data->set_class_array(class_counts);
	}

	printf("Successfully Extracted %d Unique Classes.\n", class_counts);
}
void Data_Handler::normalize_data() {
	std::vector<double> mins, maxs;
	// fill min and max lists

	Data * d = data_vector->at(0);
	for (uint8_t val : *(d->get_feature_vector()))
	{
		mins.push_back(val);
		maxs.push_back(val);
	}

	for (int i = 1; i < data_vector->size(); i++)
	{
		d = data_vector->at(i);
		for (int j = 0; j < d->get_feature_vector_size(); j++)
		{
			double value = (double)d->get_feature_vector()->at(j);
			if (value < mins.at(j)) mins[j] = value;
			if (value > maxs.at(j)) maxs[j] = value;
		}
	}

	// normalize data array
	for (int i = 0; i < data_vector->size(); i++)
	{
		Data_Handler::print_loading(false, i, data_vector->size());
		data_vector->at(i)->set_normalized_feature_vector(new std::vector<double>());
		data_vector->at(i)->set_class_vector(class_counts);
		for (int j = 0; j < data_vector->at(i)->get_feature_vector_size(); j++)
		{
			// add normalized value to normalized_feature_vector
			if (maxs[j] - mins[j] == 0)
			{
				data_vector->at(i)->append_to_feature_vector(0.0);
			}
			else {
				data_vector->at(i)->append_to_feature_vector((double)(data_vector->at(i)->get_feature_vector()->at(j) - mins[j]) / (maxs[j] - mins[j]));
			}
		}
	}
	printf("\r                     \r");
}
void Data_Handler::normalize_prediction_data() {
	std::vector<double> mins, maxs;
	// fill min and max lists

	Data * d = prediction_data->at(0);
	for (uint8_t val : *(d->get_feature_vector()))
	{
		mins.push_back(val);
		maxs.push_back(val);
	}

	for (int i = 1; i < prediction_data->size(); i++)
	{
		d = prediction_data->at(i);
		for (int j = 0; j < d->get_feature_vector_size(); j++)
		{
			double value = (double)d->get_feature_vector()->at(j);
			if (value < mins.at(j)) mins[j] = value;
			if (value > maxs.at(j)) maxs[j] = value;
		}
	}

	// normalize data array
	for (int i = 0; i < prediction_data->size(); i++)
	{
		prediction_data->at(i)->set_normalized_feature_vector(new std::vector<double>());
		prediction_data->at(i)->set_class_vector(class_counts);
		for (int j = 0; j < prediction_data->at(i)->get_feature_vector_size(); j++)
		{
			// add normalized value to normalized_feature_vector
			if (maxs[j] - mins[j] == 0)
			{
				prediction_data->at(i)->append_to_feature_vector(0.0);
			}
			else {
				prediction_data->at(i)->append_to_feature_vector((double)(prediction_data->at(i)->get_feature_vector()->at(j) - mins[j]) / (maxs[j] - mins[j]));
			}
		}
	}
}
void Data_Handler::log() {

}

int Data_Handler::get_class_count() {
	return class_counts;
}
int Data_Handler::get_data_vector_size() {
	return data_vector->size();
}
int Data_Handler::get_training_data_size() { return training_data->size(); }
int Data_Handler::get_test_data_size() { return test_data->size(); }
int Data_Handler::get_validation_data_size() { return validation_data->size(); }
int Data_Handler::get_prediction_data_size() { return prediction_data->size(); }

std::vector<Data *> * Data_Handler::get_training_data() { return training_data; }
std::vector<Data *> * Data_Handler::get_validation_data() { return validation_data; }
std::vector<Data *> * Data_Handler::get_test_data() { return test_data; }
std::vector<Data *> * Data_Handler::get_prediction_data() { return prediction_data; }
std::map<uint8_t, int> Data_Handler::get_class_map() { return class_from_int; }

uint32_t Data_Handler::format(const unsigned char* bytes)
{
	return (uint32_t)((bytes[0] << 24) |
		(bytes[1] << 16) |
		(bytes[2] << 8) |
		(bytes[3]));
}

void Data_Handler::c_only() {

	for (int i = 0; i < data_vector->size(); i++) {
		Data_Handler::print_loading(false, i, data_vector->size());
		data_vector->at(i)->c_only();
	}

	printf("\r                    \r");

	/*for (int i = 0; i < training_data->size(); i++) {
		training_data->at(i)->c_only();
	}
	for (int i = 0; i < validation_data->size(); i++) {
		validation_data->at(i)->c_only();
	}
	for (int i = 0; i < test_data->size(); i++) {
		test_data->at(i)->c_only();
	}*/
}

uint8_t print_loading_prev_val = 0;
void Data_Handler::print_loading(bool print_dots, int amt, int cap) {
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