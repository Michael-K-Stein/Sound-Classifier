#include <SFML/Graphics.hpp>

#include <iostream>
#include <sys/stat.h>
#include <time.h>
#include <vector>

#include "AudioFile.h"
#include "FFT.h"

#include <direct.h>
#define GetCurrentDir _getcwd

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


int main()
{
	std::cout << "Started WAV Analyzer" << std::endl;

	std::string WAV_File_Path;
	std::cout << "Path for WAV file: " << std::endl;
	std::cout << get_current_dir() << "\\";
	WAV_File_Path = get_current_dir() + "\\";
	std::cin >> WAV_File_Path;
	std::cout << std::endl << std::endl;

	AudioFile<double> * audioFile = new AudioFile<double>();

	if (audioFile->load(WAV_File_Path)) { std::cout << "Loaded File!" << std::endl; };
	audioFile->printSummary();
	std::cout << "CH: " << audioFile->getNumChannels() << " | " << "SAMP: " << audioFile->getNumSamplesPerChannel() * audioFile->getNumChannels() << std::endl;

	FFT * FT = new FFT();

	uint8_t channelInd = 0;
	for (int sampInd = 0; sampInd < audioFile->getNumSamplesPerChannel(); sampInd++) {
		FT->AppendToWave(audioFile->samples[channelInd][sampInd]);
	}
	int actInd = 0;
	std::cout << ++actInd << ") " << "Loaded \"" << WAV_File_Path << "\" into Fourier Transform!" << std::endl;

	uint32_t sample_count = audioFile->getNumSamplesPerChannel() / audioFile->getSampleRate();

	FILE * fVect;
	FILE * fLabel;
	
	// Create new vector and label files, or use existing ones and change the sample count
	if (!file_exists("Output/TestOne.vectors")) {
		errno_t errVects = fopen_s(&fVect, "Output/TestOne.vectors", "wb");
		errno_t errLabels = fopen_s(&fLabel, "Output/TestOne.labels", "wb");
		uint32_t intro[4] = { convert_to_big_endian(2051), convert_to_big_endian(sample_count), convert_to_big_endian(4000) };
		uint32_t introLabels[2] = { convert_to_big_endian(2049), convert_to_big_endian(sample_count) };
		fwrite((char*)intro, sizeof(char), 4 * sizeof(uint32_t), fVect);
		fwrite((char*)introLabels, sizeof(char), 2 * sizeof(uint32_t), fLabel);
	}
	else {
		errno_t errVects = fopen_s(&fVect, "Output/TestOne.vectors", "r+b");
		errno_t errLabels = fopen_s(&fLabel, "Output/TestOne.labels", "r+b");
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

	// We will define label '0' to be NOISE for this test
	//const char label[1] = { '0' };
	// We will define label '1' to be "Toca_D_Minor" for this test
	//const char label[1] = { '1' };

	/// Create an array of the frequencies
	for (int x = 0; x < sample_count; x++) {

		std::vector<Complex> * liveFreq = FT->FourierTransfer_Part(audioFile->getSampleRate(), x);

		char * freqArr = (char *)calloc(FT->MaxFrequency(), sizeof(char));

		for (int freqInd = 0; freqInd < FT->MaxFrequency(); freqInd++) {
			freqArr[freqInd] = liveFreq->at(freqInd).real();
			//printf("%d %f\n", freqInd, liveFreq->at(freqInd).real());
		}

		fwrite(freqArr, sizeof(char), FT->MaxFrequency(), fVect);
		fwrite(label, sizeof(char), 1, fLabel);
		free(freqArr);
		/*std::ofstream fout;
		char * fileName = (char *)malloc(20 * sizeof(char));
		sprintf(fileName, "Toca_D_Minor/Toca_D_Minor_Freq_%x.wavfft", x);
		fout.open(fileName, std::ios::binary |std::ios::out);
		fout.write(freqArr, FT->MaxFrequency());

		fout.close();*/
	}
	fclose(fVect);
	fclose(fLabel);

	std::string wait;
	std::cin >> wait;

	return 0;
}
