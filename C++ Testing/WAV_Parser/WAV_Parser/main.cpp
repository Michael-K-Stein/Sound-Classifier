#include "WAV.h"
#include "AudioFile.h"

int main() {
	//WAV * wav = new WAV();
	//wav->Load("../Data/1kHz.wav");
	//wav->Parse();

	AudioFile<double> * audioFile = new AudioFile<double>();
	audioFile->load("../Data/1kHz.wav");
	audioFile->printSummary();

	printf("CH | ");
	for (int i = 0; i < audioFile->getNumChannels(); i++) {
		printf(i + " | ");
	}
	printf("\n");
	for (int samp = 0; samp < audioFile->getNumSamplesPerChannel(); samp++) {
		for (int chnl = 0; chnl < audioFile->getNumChannels(); chnl++) {
			std::cout << (audioFile->samples[chnl][samp]) << " | ";
		}
		printf("\n");
	}

	while (true) {

	}
}