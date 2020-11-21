#include <SFML/Graphics.hpp>

#include <iostream>
#include <time.h>

#include "AudioFile.h"
#include "FFT.h"


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
   std::string current_working_dir(buff);
   return current_working_dir;
}


uint16_t baseLine = 900;

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

    FT->FourierTransfer(audioFile->getSampleRate());

    //FT->FourierTransfer();
    std::cout << ++actInd << ") " << "Performed Fourier Transfer on data!" << std::endl;

    std::cout << "Peak High: " << FT->peakHigh << std::endl;
    std::cout << "Peak Low: " << FT->peakLow << std::endl;
    std::cout << "Peaks: \n\t";
    for (int i = 0; i < FT->getK(); i++) { std::cout << FT->peaks->at(i).at(0) << " : " << FT->peaks->at(i).at(1) << ", \n\t"; }
    std::cout << std::endl;

    std::vector<double> * NormalizedFrequencies = FT->getNormalizedFrequency();
    std::cout << "Normalized Peaks: \n\t";
    for (int i = 0; i < FT->getK(); i++) { std::cout << FT->NormalizedPeaks->at(i).at(0) << " : " << FT->NormalizedPeaks->at(i).at(1) << "%, \n\t"; }
    std::cout << std::endl;




    sf::RenderWindow app(sf::VideoMode(800, 1000), "SFML window");

    while (app.isOpen())
    {
        // Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                app.close();
        }

        // Clear screen
        app.clear();

        // Draw the sprite

        for (int x = 0; x < audioFile->getNumSamplesPerChannel() / audioFile->getSampleRate(); x++) {

            std::vector<Complex> * liveFreq = FT->FourierTransfer_Part(audioFile->getSampleRate(), x);
            for (int xx = 0; xx < audioFile->getSampleRate(); xx++) {
                for (int freqInd = 0; freqInd < audioFile->getSampleRate() / 2; freqInd++) {
                    sf::CircleShape c(1);
                    c.setFillColor(sf::Color(255,000,255-(liveFreq->at(freqInd).real() * 2.55) ));
                    //std::cout << "SampInd: " << (double)p->sampInd << " | " << "pos: " << pos << std::endl;
                    c.setPosition((x * audioFile->getSampleRate()) + xx, baseLine - freqInd);
                    app.draw(c);
                }
            }
        }
        //app.draw();

        // Update the window
        app.display();
    }

    std::string wait;
    std::cin >>wait;

    return 0;
}
