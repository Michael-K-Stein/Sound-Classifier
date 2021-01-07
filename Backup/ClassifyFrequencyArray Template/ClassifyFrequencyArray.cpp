// ClassifyFrequencyArray.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h>
#include <fstream>

#include "AudioFile.h"


#include <direct.h>
#define GetCurrentDir _getcwd
std::string get_current_dir() {
	char buff[FILENAME_MAX]; //create string buffer to hold path
	GetCurrentDir(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}


/*int mainBROKEN()
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


	/// Create an array of the frequencies
	for (int x = 0; x < audioFile->getNumSamplesPerChannel() / audioFile->getSampleRate(); x++) {
		char * freqArr /*[4500];// /= (/*unsigned/ char *)malloc(4500 * sizeof(/*unsigned char));
		std::vector<Complex> * liveFreq = FT->FourierTransfer_Part(audioFile->getSampleRate(), x);
		std::vector<std::vector<double> * > * ordFreqOut = FT->OrderFrequencyOutputs(liveFreq);

		for (int freqInd = 0; freqInd < FT->MaxFrequency(); freqInd++) {
			freqArr[freqInd] = liveFreq->at(freqInd).real() * 2.55;
		}

		std::ofstream fout;
		char * fileName = (char *)malloc(20 * sizeof(char));
		sprintf(fileName, "Toca_D_Minor/Toca_D_Minor_Freq_%x.wavfft", x);
		fout.open(fileName, std::ios::binary |std::ios::out);
		fout.write(freqArr, 4500);

		fout.close();
	}

    std::cout << "Hello World!\n";

	return 0;
}*/

int mainpoop() {
	//mainKNN();
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
