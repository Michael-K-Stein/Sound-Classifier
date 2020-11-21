#include <iostream>
#include <time.h>

#include "AudioFile.h"
#include "FFT.h"

using namespace std;

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
std::string get_current_dir() {
   char buff[FILENAME_MAX]; //create string buffer to hold path
   GetCurrentDir( buff, FILENAME_MAX );
   string current_working_dir(buff);
   return current_working_dir;
}


int main()
{
    cout << "Started WAV Analyzer" << endl;

    string WAV_File_Path;
    cout << "Path for WAV file: " << endl;
    cout << get_current_dir() << "\\";
    WAV_File_Path = get_current_dir() + "\\";
    cin >> WAV_File_Path;
    cout << endl << endl;

    AudioFile<double> * audioFile = new AudioFile<double>();

	if (audioFile->load(WAV_File_Path)) { cout << "Loaded File!" << endl; };
	audioFile->printSummary();
    std::cout << "CH: " << audioFile->getNumChannels() << " | " << "SAMP: " << audioFile->getNumSamplesPerChannel() * audioFile->getNumChannels() << std::endl;

    FFT * FT = new FFT();

    uint8_t channelInd = 0;
    for (int sampInd = 0; sampInd < audioFile->getNumSamplesPerChannel(); sampInd++) {
        FT->AppendToWave(audioFile->samples[channelInd][sampInd]);
    }
    int actInd = 0;
    cout << ++actInd << ") " << "Loaded \"" << WAV_File_Path << "\" into Fourier Transform!" << endl;

    FT->FourierTransfer(audioFile->getSampleRate());

    //FT->FourierTransfer();
    cout << ++actInd << ") " << "Performed Fourier Transfer on data!" << endl;

    std::cout << "Peak High: " << FT->peakHigh << std::endl;
    std::cout << "Peak Low: " << FT->peakLow << std::endl;
    std::cout << "Peaks: \n\t";
    for (int i = 0; i < FT->getK(); i++) { std::cout << FT->peaks->at(i).at(0) << " : " << FT->peaks->at(i).at(1) << ", \n\t"; }
    std::cout << std::endl;

    std::vector<double> * NormalizedFrequencies = FT->getNormalizedFrequency();
    std::cout << "Normalized Peaks: \n\t";
    for (int i = 0; i < FT->getK(); i++) { std::cout << FT->NormalizedPeaks->at(i).at(0) << " : " << FT->NormalizedPeaks->at(i).at(1) << "%, \n\t"; }
    std::cout << std::endl;



    string wait;
    cin >>wait;

    return 0;
}

