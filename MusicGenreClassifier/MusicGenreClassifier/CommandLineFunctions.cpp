#include "CommandLineFunctions.h"

#include <direct.h>
#define GetCurrentDir _getcwd

#define MaximumLabelLength 255

std::string get_current_dir() {
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}
inline bool file_exists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

int getFileSize(FILE * f) { // This will move the file pointer to SEEK_SET + 0
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	fseek(f, 0, SEEK_SET);
	return size;
}

uint32_t convert_to_big_endian(uint32_t b) {
	unsigned char bytes[4];
	memcpy(bytes, &b, sizeof(b));
	return (uint32_t)((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3]));
}
uint32_t convert_to_little_endian(uint32_t b) {
	unsigned char bytes[4];
	memcpy(bytes, &b, sizeof(b));
	return (uint32_t)((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3]));
}

// Change the last character of a string into a null byte (if it's a newline).
void fix_fgets(char * inp) {
	if (inp[strlen(inp) - 1] == '\n') { inp[strlen(inp) - 1] = 0x0; }
}

// Check if an input matches a command
bool cmpcommand(char * inp, const char * command) {
	if (memcmp(inp, command, strlen(command)) == 0) {
		char fin_char = inp[strlen(command)];
		// the format specified is that a command ends with one of the following four characters.
		if (fin_char == ' ' || fin_char == '\t' || fin_char == '\n' || fin_char == '\0') {
			return true;
		}
	}
	return false;
}

// Count the amount of arguments given in an input command (as ' ' delimiter)
int CountCommandArgs(char * command) {
	char * tmpCommand = (char *)malloc((strlen(command) + 1) * sizeof(char));
	memcpy(tmpCommand, command, strlen(command) + 1);

	int count = 0;
	const char delim[2] = " "; // delimiter
	char * next_tok;
	char * tok = strtok_s(tmpCommand, delim, &next_tok);
	while (tok != NULL) {
		printf("Arg #%d: '%s'\n", count, tok);
		count++;
		tok = strtok_s(NULL, delim, &next_tok);
	}

	free(tmpCommand);

	return count;
}

// Get command input argument at index as place it into output (also allocate memory for output)
int GetCommandArg(char * command, int index, char ** output) {
	char * tmpCommand = (char *)malloc((strlen(command) + 1) * sizeof(char));
	memcpy(tmpCommand, command, strlen(command) + 1);

	int count = 0;
	const char delim[2] = " ";
	char * next_tok;
	char * tok = strtok_s(tmpCommand, delim, &next_tok);
	while (tok != NULL && count < index) {
		printf("Arg #%d: '%s'\n", count, tok);
		count++;
		tok = strtok_s(NULL, delim, &next_tok);
	}



	if (index > count) { free(tmpCommand); return -1; }

	*output = (char*)malloc((strlen(tok) + 1) * sizeof(char));
	memcpy(*output, tok, strlen(tok) + 1);

	free(tmpCommand);
	return 0;
}

// Update file paths using the user specified outputpath and the dataset global name
int UpdateFilePaths() {
	/*fn_label_map = (char*)calloc(128, sizeof(char));
	fn_data_map = (char*)calloc(128, sizeof(char));
	fn_vectors = (char*)calloc(128, sizeof(char));
	fn_labels = (char*)calloc(128, sizeof(char));*/

	sprintf_s(fn_label_map, 128, "%s/%s.label_map", OutputPath, DataSetLabel);
	sprintf_s(fn_data_map, 128, "%s/%s.data_map", OutputPath, DataSetLabel);
	sprintf_s(fn_vectors, 128, "%s/%s.vectors", OutputPath, DataSetLabel);
	sprintf_s(fn_labels, 128, "%s/%s.labels", OutputPath, DataSetLabel);

	return 0;
}
int ChangeOutputPath() {
	printf("# Please choose an output path for generated files.\n> ");
	memset(OutputPath, 0x0, 128 * sizeof(char));
	fgets(OutputPath, 127, stdin);
	fix_fgets(OutputPath);
	UpdateFilePaths();
	printf("# The data set will output to (vectors file example): '%s'\n", fn_vectors);
	return 0;
}
int ChangeDataSetName() {
	printf("# Please choose a name for this data set.\n> ");
	memset(DataSetLabel, 0x0, 128 * sizeof(char));
	fgets(DataSetLabel, 127, stdin);
	fix_fgets(DataSetLabel);
	UpdateFilePaths();
	printf("# The data set will output to (vectors file example): '%s'\n", fn_vectors);
	return 0;
}

// Count how many labels exist in the label map file.
int getExistingLabelsCount(uint8_t * existing_labels, FILE * fLabelMap) {
	int existing_labels_count = 0;
	char label_pair[MaximumLabelLength + 2];
	while (fread_s(label_pair, (MaximumLabelLength + 2) * sizeof(char), sizeof(char), MaximumLabelLength + 1, fLabelMap)) {
		uint8_t label_key = label_pair[0];
		existing_labels[existing_labels_count] = label_key;
		existing_labels_count++;
	}
	return existing_labels_count;
}

// Prints the classes and their labels, which are stored in the label map file.
int ViewClassifications() {
	printf("# The following classifications exist in the data set: \n\n");

	uint8_t existing_labels[256];
	uint8_t existing_labels_count = 0;

	// A file with a map (dictionary) of the label numbers (0-255) and their corresponding "human" definition.
	FILE * fLabelMap;
	errno_t errLabelMap;
	// A file with a map of the actual wav files which are in the vectors file.
	FILE * fDataMap;
	errno_t errDataMap;

	if (!file_exists(fn_label_map)) {
		printf("# As you can clearly see, there are none.\n");

		errLabelMap = fopen_s(&fLabelMap, fn_label_map, "wb");
		errDataMap = fopen_s(&fDataMap, fn_data_map, "wb");

		if (errDataMap || errLabelMap) { printf("# Could not open data or label map files!\n"); return -1; }
	}
	else {
		errLabelMap = fopen_s(&fLabelMap, fn_label_map, "r+b");
		errDataMap = fopen_s(&fDataMap, fn_data_map, "r+b");
		if (errDataMap || errLabelMap) { printf("# Could not open data or label map files!\n"); return -1; }

		fseek(fDataMap, 0, SEEK_END);

		/// Every 256bytes are one pair in the dictionary. Starting at byte 0.

		char label_pair[MaximumLabelLength + 2];
		while (fread_s(label_pair, (MaximumLabelLength + 2) * sizeof(char), sizeof(char), MaximumLabelLength + 1, fLabelMap)) {
			uint8_t label_key = label_pair[0];
			printf("\t> %d | %s\n", label_key, label_pair + 1);
			existing_labels[existing_labels_count] = label_key;
			existing_labels_count++;
		}

		if (existing_labels_count == 0) {
			printf("# As you can clearly see, there are none.\n");
		}
	}

	fclose(fLabelMap);
	fclose(fDataMap);

	return 0;
}

// Prints the file paths of each file in the dataset sorted by their labels.
int ViewDataFiles() {
	printf("# The following files are in the data set: \n\n");

	uint8_t existing_labels[256];
	uint8_t existing_labels_count = 0;

	// A file with a map (dictionary) of the label numbers (0-255) and their corresponding "human" definition.
	FILE * fLabelMap;
	errno_t errLabelMap;
	// A file with a map of the actual wav files which are in the vectors file.
	FILE * fDataMap;
	errno_t errDataMap;

	if (!file_exists(fn_data_map)) {
		printf("# As you can clearly see, there are none.\n");

		errLabelMap = fopen_s(&fLabelMap, fn_label_map, "wb");
		errDataMap = fopen_s(&fDataMap, fn_data_map, "wb");

		if (errDataMap || errLabelMap) { printf("# Could not open data or label map files!\n"); return -1; }
	}
	else {
		errLabelMap = fopen_s(&fLabelMap, fn_label_map, "r+b");
		errDataMap = fopen_s(&fDataMap, fn_data_map, "r+b");
		if (errDataMap || errLabelMap) { printf("# Could not open data or label map files!\n"); return -1; }

		int size = getFileSize(fDataMap); fseek(fDataMap, 0, SEEK_SET);
		char * file_data = (char*)malloc(size + 1);
		int read_count = fread_s(file_data, size, sizeof(char), size, fDataMap);
		if (read_count == 0) {
			printf("# Error reading file.\n"); return -1;
		}


		std::map<uint8_t, char*> label_data_map;

		/// Every 256bytes are one pair in the dictionary. Starting at byte 0.

		char label_pair[MaximumLabelLength + 2];
		while (fread_s(label_pair, (MaximumLabelLength + 2) * sizeof(char), sizeof(char), MaximumLabelLength + 1, fLabelMap)) {
			uint8_t label_key = label_pair[0];
			//existing_labels[existing_labels_count] = label_key;
			//label_data_map.insert(std::pair<uint8_t,char*>(label_key, label_pair + 1));

			printf("\t> %u | %s\n", label_key, label_pair + 1);

			char * live_data = file_data;

			int pos = 0;
			while (pos < size) {
				uint8_t data_key = live_data[pos];
				if (data_key == label_key) {
					printf("\t|--> %s\n", live_data + pos + 1);
				}
				pos += 2 + strlen(live_data + pos + 1);
			}

			existing_labels_count++;
		}

		if (existing_labels_count == 0) {
			printf("# As you can clearly see, there are none.\n");
		}
		/*else {

			for (int i = 0; i < existing_labels_count; i++) {

				uint8_t key = existing_labels[i];
				printf("\t> %u | %s\n", key, label_data_map.at(key));

				char * live_data = file_data;

				int pos = 0;
				while (pos < size) {
					uint8_t data_key = live_data[pos];
					if (data_key == key) {
						printf("\t|--> %s\n", live_data + pos + 1);
					}
					pos += 2 + strlen(live_data + pos + 1);
				}
			}

		}*/

		free(file_data);
	}

	fclose(fLabelMap);
	fclose(fDataMap);

	return 0;
}
int AddDataToSet() {

	std::string WAV_File_Path;
	std::cout << "# Path for WAV file: " << std::endl;
	std::cout << get_current_dir() << "\\";
	WAV_File_Path = get_current_dir() + "\\";
	char WAV_File_Path_buffer[256];
	fgets(WAV_File_Path_buffer, sizeof(WAV_File_Path_buffer), stdin);
	fix_fgets(WAV_File_Path_buffer);
	for (int i = 0; i < strlen(WAV_File_Path_buffer); i++) {
		WAV_File_Path += WAV_File_Path_buffer[i];
	}
	std::cout << std::endl << std::endl;

	AudioFile<double> * audioFile = new AudioFile<double>();

	if (audioFile->load(WAV_File_Path)) {
		std::cout << "# Loaded File!" << std::endl;
		audioFile->printSummary();
		std::cout << "# CH: " << audioFile->getNumChannels() << " | " << "SAMP: " << audioFile->getNumSamplesPerChannel() * audioFile->getNumChannels() << std::endl;

		audioFile->ConcactChannels();

		FFT * FT = new FFT();

		uint8_t channelInd = 0;
		for (int sampInd = 0; sampInd < audioFile->getNumSamplesPerChannel(); sampInd++) {
			FT->AppendToWave(audioFile->samples[channelInd][sampInd]);
		}
		int actInd = 0;
		std::cout << "\t" << ++actInd << ") " << "Loaded \"" << WAV_File_Path << "\" into Fourier Transform!" << std::endl;

		uint32_t sample_count = audioFile->getNumSamplesPerChannel() / audioFile->getSampleRate();

		// A file with a map (dictionary) of the label numbers (0-255) and their corresponding "human" definition.
		FILE * fLabelMap;
		// A file with a map of the actual wav files which are in the vectors file.
		FILE * fDataMap;

		errno_t errLabelMap = fopen_s(&fLabelMap, fn_label_map, "r+b");
		errno_t errDataMap = fopen_s(&fDataMap, fn_data_map, "r+b");

		if (errDataMap || errLabelMap) { printf("# Could not open Data or Label map files!\n"); return -1; }

		// Create new vector and label files, or use existing ones and change the sample count
		FILE * fVect;
		FILE * fLabel;
		if (!file_exists(fn_vectors)) {
			errno_t errVects = fopen_s(&fVect, fn_vectors, "wb");
			errno_t errLabels = fopen_s(&fLabel, fn_labels, "wb");
			uint32_t intro[3] = { convert_to_big_endian(2612), convert_to_big_endian(sample_count), convert_to_big_endian(4000) };
			uint32_t introLabels[2] = { convert_to_big_endian(2211), convert_to_big_endian(sample_count) };
			fwrite((char*)intro, sizeof(char), 4 * sizeof(uint32_t), fVect);
			fwrite((char*)introLabels, sizeof(char), 2 * sizeof(uint32_t), fLabel);
		}
		else {
			errno_t errVects = fopen_s(&fVect, fn_vectors, "r+b");
			errno_t errLabels = fopen_s(&fLabel, fn_labels, "r+b");
			uint32_t old_sample_count = 0;
			fseek(fLabel, sizeof(uint32_t), SEEK_SET);
			fread_s(&old_sample_count, sizeof(old_sample_count), sizeof(uint32_t), 1, fLabel);
			old_sample_count = convert_to_little_endian(old_sample_count);
			old_sample_count += sample_count;
			old_sample_count = convert_to_big_endian(old_sample_count);
			fseek(fLabel, sizeof(uint32_t), SEEK_SET);
			fseek(fVect, sizeof(uint32_t), SEEK_SET);
			fwrite(&old_sample_count, sizeof(uint32_t), 1, fLabel);
			fwrite(&old_sample_count, sizeof(uint32_t), 1, fVect);
			fseek(fLabel, 0, SEEK_END);
			fseek(fVect, 0, SEEK_END);
		}


		printf("Please enter the label (as uint8) for this sound.\n> ");
		char label_buffer[16];
		fgets(label_buffer, sizeof(label_buffer), stdin);
		fix_fgets(label_buffer);

		long lab_long = strtol(label_buffer, NULL, 10);

		uint8_t lab = 0;
		memcpy(&lab, &lab_long, sizeof(uint8_t));
		char label[1] = { lab };


		/*char * label_buffer_temp = (char*)malloc(strlen(label_buffer));
		memcpy(label_buffer_temp, label_buffer, strlen(label_buffer));
		printf("\n\nGot label! %u\n\n", label_buffer_temp[0]);

		sprintf_s(label, sizeof(label), "%u", label_buffer_temp[0]);

		free(label_buffer_temp);*/

		uint8_t existing_labels[256];
		int existing_labels_count = getExistingLabelsCount(existing_labels, fLabelMap);

		fseek(fDataMap, 0, SEEK_END);
		fseek(fLabelMap, 0, SEEK_END);

		fwrite(label, sizeof(uint8_t), 1, fDataMap);
		/*char * WAV_File_Path_c_str = (char*)calloc(WAV_File_Path.length(), sizeof(char));
		strncpy_s(WAV_File_Path_c_str, WAV_File_Path.length(), WAV_File_Path.c_str(), WAV_File_Path.length());
		fwrite(WAV_File_Path_c_str, sizeof(char), WAV_File_Path.length(), fDataMap);*/
		fwrite(WAV_File_Path.c_str(), sizeof(char), WAV_File_Path.length(), fDataMap);
		fwrite(OutputPath + 127, sizeof(char), 1, fDataMap);

		bool exists = false;
		if (existing_labels_count > 0) {

			int i = 0;
			while (i < 256 && !exists) {
				exists = exists || existing_labels[i] == lab;
				i++;
			}
		}
		if (!exists) {
			printf("# This is a new label, please describe it. (in 255 or less characters)\n> ");
			char label_description_buffer[MaximumLabelLength + 1];
			fgets(label_description_buffer, MaximumLabelLength, stdin);
			fix_fgets(label_description_buffer);

			fwrite(label, 1, 1, fLabelMap);
			fwrite(label_description_buffer, sizeof(char), MaximumLabelLength, fLabelMap);

			printf("# Added the pair: {%u, %s} to the label_map.\n", lab, label_description_buffer);
		}

		printf("# Now loading the file with the label %d.\n", lab);

		/// Create an array of the frequencies
		for (int x = 0; x < sample_count; x++) {

			std::vector<Complex> * liveFreq = FT->FourierTransfer_Part(audioFile->getSampleRate(), x);

			char * freqArr = (char *)calloc(FT->MaxFrequency(), sizeof(char));

			for (int freqInd = 0; freqInd < FT->MaxFrequency(); freqInd++) {
				freqArr[freqInd] = liveFreq->at(freqInd).real();
			}

			fwrite(freqArr, sizeof(char), FT->MaxFrequency(), fVect);
			fwrite(label, sizeof(char), 1, fLabel);
			free(freqArr);
		}
		fclose(fVect);
		fclose(fLabel);
		fclose(fLabelMap);
		fclose(fDataMap);

		delete FT;

		printf("# Appended sound!\n");
	}

	delete audioFile;

	return 0;
}

// Initialize a new dataset
int Initialize() {
	FILE * fLabelMap;
	errno_t errLabelMap;
	FILE * fDataMap;
	errno_t errDataMap;
	errLabelMap = fopen_s(&fLabelMap, fn_label_map, "wb");
	errDataMap = fopen_s(&fDataMap, fn_data_map, "wb");
	fclose(fLabelMap);
	fclose(fDataMap);

	return 0;
}

// predict the label of a file using the Classifier
/*
	The predict function can take arguments such as:
	1) a file path.
		The first input can be the relative path to a vectors file which you want to use instead of processing a wav file into a vectors file.
	2) the '-f' argument.
		This is passed into the classifier as 'fast' which tells the program to only check some of the data points (arbitrarily) and not all of them.
		This obviously results in faster execution time (10x faster - since we take only a tenth of the vectors), but there is a penalty to the accuracy of the results. Though the final prediction is still usually correct.
*/
int Predict(char * command) {
	int argc = CountCommandArgs(command);
	printf("Arg count: %d\n", argc);
	if (argc == 0) {
		char predictOutputFile[256];
		char predictOutputFile_tmp[128];
		printf("Name your prediction file: \n> ");
		fgets(predictOutputFile_tmp, sizeof(predictOutputFile_tmp), stdin);
		fix_fgets(predictOutputFile_tmp);
		sprintf_s(predictOutputFile, sizeof(predictOutputFile), "%s/%s.vectors", OutputPath, predictOutputFile_tmp);

		std::string WAV_File_Path;
		std::cout << "# Path for WAV file: " << std::endl;
		std::cout << get_current_dir() << "\\";
		WAV_File_Path = get_current_dir() + "\\";
		char WAV_File_Path_buffer[256];
		fgets(WAV_File_Path_buffer, sizeof(WAV_File_Path_buffer), stdin);
		fix_fgets(WAV_File_Path_buffer);
		for (int i = 0; i < strlen(WAV_File_Path_buffer); i++) {
			WAV_File_Path += WAV_File_Path_buffer[i];
		}
		std::cout << std::endl << std::endl;

		AudioFile<double> * audioFile = new AudioFile<double>();

		// Parse audio file into FFT and export it as vectors into a vectors file
		if (audioFile->load(WAV_File_Path)) {
			std::cout << "# Loaded File!" << std::endl;
			audioFile->printSummary();
			std::cout << "# CH: " << audioFile->getNumChannels() << " | " << "SAMP: " << audioFile->getNumSamplesPerChannel() * audioFile->getNumChannels() << std::endl;

			FFT * FT = new FFT();

			uint8_t channelInd = 0;
			for (int sampInd = 0; sampInd < audioFile->getNumSamplesPerChannel(); sampInd++) {
				FT->AppendToWave(audioFile->samples[channelInd][sampInd]);
			}
			int actInd = 0;
			std::cout << "\t" << ++actInd << ") " << "Loaded \"" << WAV_File_Path << "\" into Fourier Transform!" << std::endl;

			uint32_t sample_count = audioFile->getNumSamplesPerChannel() / audioFile->getSampleRate();

			FILE * fVect;
			errno_t errVects;
			if (!file_exists(predictOutputFile)) {
				errVects = fopen_s(&fVect, predictOutputFile, "wb");
				uint32_t intro[3] = { convert_to_big_endian(2612), convert_to_big_endian(sample_count), convert_to_big_endian(4000) };
				fwrite((char*)intro, sizeof(char), 3 * sizeof(uint32_t), fVect);
			}
			else {
				errVects = fopen_s(&fVect, predictOutputFile, "r+b");
				uint32_t old_sample_count = 0;
				fread_s(&old_sample_count, sizeof(old_sample_count), sizeof(uint32_t), 1, fVect);
				old_sample_count = convert_to_little_endian(old_sample_count);
				old_sample_count += sample_count;
				old_sample_count = convert_to_big_endian(old_sample_count);
				fseek(fVect, sizeof(uint32_t), SEEK_SET);
				fwrite(&old_sample_count, sizeof(uint32_t), 1, fVect);
				fseek(fVect, 0, SEEK_END);
			}
			if (errVects) { printf("Could not find nor create prediction file!\n"); return -1; }

			/// Create an array of the frequencies
			for (int x = 0; x < sample_count; x++) {

				std::vector<Complex> * liveFreq = FT->FourierTransfer_Part(audioFile->getSampleRate(), x);

				char * freqArr = (char *)calloc(FT->MaxFrequency(), sizeof(char));

				for (int freqInd = 0; freqInd < FT->MaxFrequency(); freqInd++) {
					freqArr[freqInd] = liveFreq->at(freqInd).real();
				}

				fwrite(freqArr, sizeof(char), FT->MaxFrequency(), fVect);
				free(freqArr);
			}
			fclose(fVect);

			delete FT;
		}
		delete audioFile;

		printf("# Created prediction file!!\n");

		data_handler->read_predict_feature_vector(predictOutputFile);
	}
	else if (argc == 1) {
		char * predictOutputFile;
		if (GetCommandArg(command, 0, &predictOutputFile) != 0) { printf("# Could not find argument!\n"); return -1; }
		data_handler->read_predict_feature_vector(predictOutputFile);
	}
	else {
		printf("# Unrecognized argument count!\n");
	}

	data_handler->normalize_prediction_data();

	for (int i = 0; i < data_handler->get_prediction_data_size(); i++) {
		data_handler->get_prediction_data()->at(i)->c_only();
	}

	network->set_prediction_data(data_handler->get_prediction_data());

	int prediction_output = network->real_predict();


	FILE * fLabelMap;
	errno_t errLabelMap = fopen_s(&fLabelMap, fn_label_map, "r+b");
	if (errLabelMap) { printf("# Could not open label map file!\n"); return -1; }

	char label_pair[MaximumLabelLength + 2];
	while (fread_s(label_pair, (MaximumLabelLength + 2) * sizeof(char), sizeof(char), MaximumLabelLength + 1, fLabelMap)) {
		uint8_t label_key = label_pair[0];
		if (label_key == prediction_output) {
			printf("The prediction for this file is: \n\t%d | %s\n", prediction_output, label_pair + 1);
		}
	}

	fclose(fLabelMap);


	network->set_training_data(data_handler->get_training_data());
	network->set_validation_data(data_handler->get_validation_data());
	network->set_test_data(data_handler->get_test_data());

	return 0;
}

int InitNetwork() {
	if (data_handler == NULL) {
		printf("You must have a data_handler before initializing a network!\n");
		return 2;
	}

	std::vector<int> hidden_layers = { 420, 32 };
	network = new Network(
		hidden_layers,
		data_handler->get_training_data()->at(0)->get_feature_array_size(),
		data_handler->get_class_count(),
		0.25);

	network->set_training_data(data_handler->get_training_data());
	network->set_validation_data(data_handler->get_validation_data());
	network->set_test_data(data_handler->get_test_data());

	network->c_only();

	return 0;
}

int LoadNetwork(char * command) {
	if (network != NULL) {
		printf("You currently have a network with an accuracy of %.4f%% loaded. Please dereference it before proceeding!\n", network->test_performance * 100.0);
		return 2;
	}

	FILE * f;
	errno_t err = fopen_s(&f, command, "rb");
	if (err == 0) {
		// Successfully opened file for reading

		/// IMPORT
		unsigned long long file_size = 0;
		fread(&file_size, sizeof(file_size), 1, f);
		fseek(f, 0, SEEK_SET);
		char * buffer_read = (char*)calloc(file_size, sizeof(char));

		uint32_t read = fread(buffer_read, sizeof(char), file_size, f);

		fclose(f);

		network = new Network(0.25);
		network->import_network(buffer_read);
		free(buffer_read);

		network->set_training_data(data_handler->get_training_data());
		network->set_validation_data(data_handler->get_validation_data());
		network->set_test_data(data_handler->get_test_data());


	}
	else {
		printf("Failed to open or read file '%s' which was requested as a network import file!", command);
	}

	return 0;
}

int ExportNetwork(char * command) {
	network->test_c();

	char * buffer;
	int total_size = network->export_network(&buffer);

	FILE * f;
	errno_t err = fopen_s(&f, command, "wb");
	fwrite(buffer, sizeof(char), total_size, f);
	fclose(f);

	return 0;
}

int TrainNetwork(char * command) {
	if (network == NULL) {
		printf("You must first load or initialize a network before training it!\n");
		return 2;
	}

	long epochs = 0;
	epochs = strtol(command, NULL, 10);

	printf("Now training %d times...\n", epochs);

	for (int i = 0; i < epochs; i++) {
		printf("Training error @ iteration %d: %.4f\n", i, network->train_c());
	}

	return 0;
}

int ThreadTrainNetwork(char * command) {
	if (network == NULL) {
		printf("You must first load or initialize a network before training it!\n");
		return 2;
	}

	long epochs = 0;
	epochs = strtol(command, NULL, 10);

	printf("Now training %d times...\n", epochs);

	auto lamba = [&](int target) {
		for (int i = 0; i < epochs; i++) {
			printf("Thread #%d => Training error @ iteration %d: %.4f\n", target, i, network->train_c(target));
		}
	};

	std::vector<std::thread> threads;

	for (int target = 0; target < data_handler->get_class_count(); target++)
	{
		threads.emplace_back(std::thread(lamba, target));
	}

	for (auto &th : threads)
	{
		th.join();
	}

	return 0;
}

int ValidateNetwork() {
	if (network == NULL) {
		printf("You must first load or initialize a network before validating it!\n");
		return 2;
	}

	network->set_training_data(data_handler->get_training_data());
	network->set_validation_data(data_handler->get_validation_data());
	network->set_test_data(data_handler->get_test_data());

	printf("Validation performance: %.4f%%\n", 100.0 * network->validate_c());

	return 0;
}

int TestNetwork() {
	if (network == NULL) {
		printf("You must first load or initialize a network before validating it!\n");
		return 2;
	}

	network->set_training_data(data_handler->get_training_data());
	network->set_validation_data(data_handler->get_validation_data());
	network->set_test_data(data_handler->get_test_data());

	printf("Test performance: %.4f%%\n", 100.0 * network->test_c());

	return 0;
}

int LoadDataHandler() {
	if (data_handler != NULL) { delete data_handler; }
	data_handler = new Data_Handler();
	data_handler->read_feature_vector(fn_vectors);
	data_handler->read_feature_labels(fn_labels);
	data_handler->count_classes();
	data_handler->normalize_data();
	data_handler->split_data();
	data_handler->c_only();

	return 0;
}
int ResetDataHandler() {
	if (data_handler != NULL) { delete data_handler; }
	data_handler = new Data_Handler();

	return 0;
}
int LoadPredictFile(char * command) {
	if (data_handler == NULL) {
		printf("There is no data_handler, please initialize one before loading a predict file!\n");
		return 2;
	}

	data_handler->read_predict_feature_vector(command);
	data_handler->normalize_prediction_data();

	for (int i = 0; i < data_handler->get_prediction_data_size(); i++) {
		data_handler->get_prediction_data()->at(i)->c_only();
	}

	return 0;
}

int ExitProgram() {
	free(fn_data_map);
	free(fn_label_map);
	free(fn_labels);
	free(fn_vectors);
	exit(0);
	return 0;
}
int Help(bool ext) {
	printf("You can use the following commands:\n");
	printf("\t> help\t\t\t| To see this menu.\n");
	printf("\t> help extended\t\t| To see a full list of commands.\n");
	printf("\t> init\t\t\t| Initialize dataset files. You must do this first when creating a new dataset!\n");
	printf("\t> change data set name\t| To change the name given to output files.\n");
	if (ext) {
		printf("\t> change dataset name\t| To change the name given to output files.\n");
		printf("\t> change data name\t| To change the name given to output files.\n");
	}
	printf("\t> change output\t\t| To change the output path of created files.\n");
	printf("\t> list classes\t\t| To see which classifications exist in the data set.\n");
	if (ext) {
		printf("\t> list classifications\t| To see which classifications exist in the data set.\n");
		printf("\t> list class\t\t| To see which classifications exist in the data set.\n");
		printf("\t> view classes\t\t| To see which classifications exist in the data set.\n");
		printf("\t> view classifications\t| To see which classifications exist in the data set.\n");
		printf("\t> view class\t\t| To see which classifications exist in the data set.\n");
	}
	printf("\t> list files\t\t| To see a sorted list of the files which have been loaded into the dataset.\n");
	if (ext) {
		printf("\t> list data\t\t| To see a sorted list of the files which have been loaded into the dataset.\n");
		printf("\t> view files\t\t| To see a sorted list of the files which have been loaded into the dataset.\n");
		printf("\t> view data\t\t| To see a sorted list of the files which have been loaded into the dataset.\n");
	}
	printf("\t> add file\t\t| To add another file into the dataset.\n");
	if (ext) {
		printf("\t> add data\t\t| To add another file into the dataset.\n");
	}
	printf("\t> predict\t\t| Predict the genre of a wav file.\n");
	printf("\t> load predict file\t| Load a prediction file into the data handler.\n");
	printf("\t> init data handler\t| Loads the database into the data handler (HDD => RAM).\n");
	printf("\t> reset data handler\t| Resets the existing data handler or creates a blank one.\n");
	printf("\t> load net {0}\t\t| Load a network from file.\n");
	if (ext) { printf("\t> import net {0}\t\t| Load a network from file.\n"); }
	printf("\t> export net {0}\t\t| Exports the network to a file.\n");
	printf("\t> init net\t\t| Initializes a random network.\n");
	printf("\t> train net {0}\t\t| Train the nework a certain amount of times.\n");
	printf("\t> thread train net {0}\t| Train the nework a certain amount of times on separate threads (beta).\n");
	printf("\t> valid net\t\t| Validate the network.\n");
	if (ext) { printf("\t> validate net\t| Validate the network.\n"); }
	printf("\t> test net\t\t| Test the network.\n");
	printf("\t> exit\t\t\t| To close the program.\n");

	return 0;
}

// Gets user input and compares it to valid commands
int ProccessCommands() {
	printf("> ");
	char input[1024];
	fgets(input, sizeof(input), stdin);
	fix_fgets(input);

	if (cmpcommand(input, "help") || cmpcommand(input, "help extended") || cmpcommand(input, "help ext")) { return Help(cmpcommand(input, "help extended") || cmpcommand(input, "help ext")); }
	if (cmpcommand(input, "change output")) { return ChangeOutputPath(); }
	if (cmpcommand(input, "change data set name") || cmpcommand(input, "change dataset name") || cmpcommand(input, "change data name")) { return ChangeOutputPath(); }
	if (cmpcommand(input, "list classifications") || cmpcommand(input, "view classifications") || cmpcommand(input, "view class") || cmpcommand(input, "list class") || cmpcommand(input, "view classes") || cmpcommand(input, "list classes")) { return ViewClassifications(); }
	if (cmpcommand(input, "list files") || cmpcommand(input, "list data") || cmpcommand(input, "view files") || cmpcommand(input, "view data")) { return ViewDataFiles(); }
	if (cmpcommand(input, "add file") || cmpcommand(input, "add data")) { return AddDataToSet(); }
	if (cmpcommand(input, "predict")) { return Predict(input + strlen("predict") + 1); }
	if (cmpcommand(input, "load predict file")) { return LoadPredictFile(input + strlen("load predict file") + 1); }
	if (cmpcommand(input, "init data handler")) { return LoadDataHandler(); }
	if (cmpcommand(input, "reset data handler")) { return ResetDataHandler(); }
	if (cmpcommand(input, "load net")) { return LoadNetwork(input + strlen("load net") + 1); }
	if (cmpcommand(input, "import net")) { return LoadNetwork(input + strlen("import net") + 1); }
	if (cmpcommand(input, "export net")) { return ExportNetwork(input + strlen("export net") + 1); }
	if (cmpcommand(input, "init net")) { return InitNetwork(); }
	if (cmpcommand(input, "train net")) { return TrainNetwork(input + strlen("train net") + 1); }
	if (cmpcommand(input, "thread train net")) { return ThreadTrainNetwork(input + strlen("thread train net") + 1); }
	if (cmpcommand(input, "validate net") || cmpcommand(input, "valid net")) { return ValidateNetwork(); }
	if (cmpcommand(input, "test net")) { return TestNetwork(); }
	if (cmpcommand(input, "init") || cmpcommand(input, "initialize")) { return Initialize(); }
	if (cmpcommand(input, "exit") || cmpcommand(input, "close")) { return ExitProgram(); }

	return 1;
}