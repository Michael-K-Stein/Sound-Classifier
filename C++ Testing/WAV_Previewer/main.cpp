#include <SFML/Graphics.hpp>
#include "AudioFile.h"
#include <algorithm>

uint8_t dstMult = 90;
uint16_t baseLine = 100;
uint16_t baseLine2 = 100;
uint16_t baseLine3 = 300;
uint64_t xShift = 0;

int width = 1000;

sf::Color colors[] = { sf::Color::Red, sf::Color::Blue, sf::Color::Magenta, sf::Color::Green, sf::Color::Yellow };

struct peak {
    double sampInd;
    double sampVal;
};

float MAXSECONDS = 10.0f;

int main()
{
    AudioFile<double> * audioFile = new AudioFile<double>();
	//audioFile->load("../../Data Sets/Music/Ode To Joy.wav");
	//audioFile->load("../../Data Sets/Speech/HelloMichael.wav");
	audioFile->load("Data/Pitch Dif Testing.wav");
	audioFile->printSummary();
    std::cout << "CH: " << audioFile->getNumChannels() << " | " << "SAMP: " << audioFile->getNumSamplesPerChannel() * audioFile->getNumChannels() << std::endl;

    uint64_t maxSamps = (audioFile->getNumSamplesPerChannel() / audioFile->getLengthInSeconds()) * MAXSECONDS;
    uint64_t realSamps = std::min(maxSamps, (uint64_t)audioFile->getNumSamplesPerChannel());

    std::vector<std::vector<peak *> * > * peaks = new std::vector<std::vector<peak *> * >;

    uint64_t impCount = 0;
    for (uint64_t chId = 0; chId < audioFile->getNumChannels(); chId++) {
        peaks->push_back(new std::vector<peak *>);
        for (uint64_t sam = 0; sam < realSamps; sam++) {
            if ((audioFile->samples[chId][sam - 1] < audioFile->samples[chId][sam] && audioFile->samples[chId][sam + 1] < audioFile->samples[chId][sam]) ||
                (audioFile->samples[chId][sam - 1] > audioFile->samples[chId][sam] && audioFile->samples[chId][sam + 1] > audioFile->samples[chId][sam])
            ){
                impCount++;
                peak * p = new peak;
                p->sampInd = sam;
                p->sampVal = audioFile->samples[chId][sam];
                peaks->at(chId)->push_back(p);

            }
        }
    }



    std::cout << "Important Points: " << impCount << std::endl;
    std::cout << "Reduction: " << (1.0 - (double)((double)impCount / ( audioFile->getNumChannels() * (double)realSamps))) * 100.0 << "%" << std::endl;

    //while (audioFile->samples[0][xShift] == 0) { xShift+=1000; }

    // Create the main window
    sf::RenderWindow app(sf::VideoMode(width, 200), "WAV Previewer");
    sf::RenderWindow app2(sf::VideoMode(width, 400), "WAV Previewer");


    while (true) {
        app2.clear();
        for (int channelInd = 0; channelInd < audioFile->getNumChannels(); channelInd++) {
            //std::cout << "Size: " << peaks->at(channelInd)->size() << std::endl;
            for (uint64_t ind = 0; ind < peaks->at(channelInd)->size(); ind++ ) {
                peak * p = peaks->at(channelInd)->at(ind);

                sf::CircleShape c(1); c.setFillColor(colors[(channelInd + 2) % 5]);
                double pos = width * (double)((double)p->sampInd / realSamps);
                //std::cout << "SampInd: " << (double)p->sampInd << " | " << "pos: " << pos << std::endl;
                c.setPosition(pos, baseLine2 + (audioFile->samples[channelInd][p->sampInd] * (dstMult)));
                app2.draw(c);
            }

            for (uint64_t ind = 0; ind < realSamps; ind++ ) {
                sf::CircleShape c(1); c.setFillColor(colors[(channelInd + 2) % 5]);
                double pos = width * (double)((double)ind / realSamps);
                //std::cout << "SampInd: " << (double)p->sampInd << " | " << "pos: " << pos << std::endl;
                c.setPosition(pos, baseLine3 + (audioFile->samples[channelInd][ind] * (dstMult)));
                app2.draw(c);
            }
        }
        app2.display();

        std::cout << "Finished drawing app2" << std::endl;
    }
	// Start the game loop
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

        // Draw
        /*double t = (double)( xShift / (double)audioFile->getSampleRate() );
        double t2 = (double)( (xShift + 6000) / (double)audioFile->getSampleRate() );
        printf("%F sec -> %F\n", t, t2);// std::cout << () << " sec" << std::endl;
        for (int channelInd = 0; channelInd < audioFile->getNumChannels(); channelInd++) {
            uint32_t x = 0;
            uint32_t tShift = 0;
            for (int sampInd = 0; sampInd < audioFile->getNumSamplesPerChannel(); sampInd++) {
                sf::CircleShape c(1); c.setFillColor(colors[channelInd % 5]);
                c.setPosition(x/5, baseLine + (audioFile->samples[channelInd][sampInd + xShift + tShift] * dstMult));
                app.draw(c);
                //audioFile->samples[chnl][samp]
                x+=5;
                tShift+=4;
                if (x > 2000) { xShift++; break; }
            }
        }*/

        sf::Vertex line[2];
        line[0].position = sf::Vector2f(0, baseLine);
        line[0].color  = sf::Color::White;
        line[1].position = sf::Vector2f(1000, baseLine);
        line[1].color = sf::Color::White;
        app.draw(line, 2, sf::Lines);


        for (int channelInd = 0; channelInd < audioFile->getNumChannels(); channelInd++) {
            uint32_t x = 0;
            for (int sampInd = 0; sampInd < audioFile->getNumSamplesPerChannel(); sampInd++) {
                if ((audioFile->samples[channelInd][sampInd + xShift - 1] < audioFile->samples[channelInd][sampInd + xShift] && audioFile->samples[channelInd][sampInd + xShift + 1] < audioFile->samples[channelInd][sampInd + xShift]) ||
                    (audioFile->samples[channelInd][sampInd + xShift - 1] > audioFile->samples[channelInd][sampInd + xShift] && audioFile->samples[channelInd][sampInd + xShift + 1] > audioFile->samples[channelInd][sampInd + xShift])
                    ){
                    sf::CircleShape c(3); c.setFillColor(sf::Color::White);
                    c.setPosition(x, baseLine + (audioFile->samples[channelInd][sampInd + xShift] * (dstMult)));
                    app.draw(c);

                } else {
                    sf::CircleShape c(1); c.setFillColor(colors[(channelInd + 2) % 5]);
                    c.setPosition(x, baseLine + (audioFile->samples[channelInd][sampInd + xShift] * (dstMult)));
                    app.draw(c);
                }
                //audioFile->samples[chnl][samp]
                x+=1;
                if (x > 2000) { xShift++; break; }
            }
        }



        // Update the window
        app.display();
    }

    return EXIT_SUCCESS;
}
