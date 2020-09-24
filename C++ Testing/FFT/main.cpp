#include <SFML/Graphics.hpp>
#include <complex>
#include <iostream>
#include <valarray>

#include "FFT.h"

uint8_t dstMult = 50;
uint16_t baseLine = 100;
uint16_t baseLine2 = 300;
uint16_t baseLineY = 500;
uint16_t baseLineN1 = 700;
uint16_t baseLineN2 = 800;

uint16_t baseLines[4] = { baseLine, baseLine2, baseLineN1, baseLineN2 };

bool kilo = true;

const double PI = 3.141592653589793238460;

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;


// Cooley–Tukey FFT (in-place, divide-and-conquer)
// Higher memory requirements and redundancy although more intuitive
void fft(CArray& x)
{
    const size_t N = x.size();
    if (N <= 1) return;

    // divide
    CArray even = x[std::slice(0, N/2, 2)];
    CArray  odd = x[std::slice(1, N/2, 2)];

    // conquer
    fft(even);
    fft(odd);

    // combine
    for (size_t k = 0; k < N/2; ++k)
    {
        Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k];
        x[k    ] = even[k] + t;
        x[k+N/2] = even[k] - t;
    }
}

// Cooley-Tukey FFT (in-place, breadth-first, decimation-in-frequency)
// Better optimized but less intuitive
// !!! Warning : in some cases this code make result different from not optimased version above (need to fix bug)
// The bug is now fixed @2017/05/30
/*void fft(CArray &x)
{
	// DFT
	unsigned int N = x.size(), k = N, n;
	double thetaT = 3.14159265358979323846264338328L / N;
	Complex phiT = Complex(cos(thetaT), -sin(thetaT)), T;
	while (k > 1)
	{
		n = k;
		k >>= 1;
		phiT = phiT * phiT;
		T = 1.0L;
		for (unsigned int l = 0; l < k; l++)
		{
			for (unsigned int a = l; a < N; a += n)
			{
				unsigned int b = a + k;
				Complex t = x[a] - x[b];
				x[a] += x[b];
				x[b] = t * T;
			}
			T *= phiT;
		}
	}
	// Decimate
	unsigned int m = (unsigned int)log2(N);
	for (unsigned int a = 0; a < N; a++)
	{
		unsigned int b = a;
		// Reverse bits
		b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
		b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
		b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
		b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
		b = ((b >> 16) | (b << 16)) >> (32 - m);
		if (b > a)
		{
			Complex t = x[a];
			x[a] = x[b];
			x[b] = t;
		}
	}
	//// Normalize (This section make it not working correctly)
	//Complex f = 1.0 / sqrt(N);
	//for (unsigned int i = 0; i < N; i++)
	//	x[i] *= f;
}*/

// inverse fft (in-place)
void ifft(CArray& x)
{
    // conjugate the complex numbers
    x = x.apply(std::conj);

    // forward fft
    fft( x );

    // conjugate the complex numbers again
    x = x.apply(std::conj);

    // scale the numbers
    x /= x.size();
}

int DemoFFT(Complex test)
{
    //Complex test[] = { 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0 };
    CArray data(test, 8);

    // forward fft
    fft(data);

    std::cout << "fft" << std::endl;
    for (int i = 0; i < 8; ++i)
    {
        std::cout << data[i] << std::endl;
    }

    // inverse fft
    ifft(data);

    std::cout << std::endl << "ifft" << std::endl;
    for (int i = 0; i < 8; ++i)
    {
        std::cout << data[i] << std::endl;
    }
    return 0;
}


int main()
{
    FFT * FT = new FFT();


    sf::Font f;
    f.loadFromFile("fonts/arial.ttf");

    //double orig[10000];
    //Complex test[10000];
    //int ind = 0;
    for (double i = 0; i < 1; i+= (1.0 / 44000.0)) {
        std::complex<double> nP (1.0 * sin(6666 * i) + 0.0 * sin(200 * i));
        FT->AppendToWave(nP.real());
        //test[ind] = nP;
        //orig[ind] = nP.real();
        //ind++;
    }

    //const Complex test[] = { 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0 };
    //DemoFFT(test);
/*
    CArray data(test, sizeof(test) / sizeof(test[0]));
    fft(data);*/

    // Create the main window
    sf::RenderWindow app(sf::VideoMode(1000, 850), "SFML window");

    FT->FourierTransfer();

    std::cout << "opened window" << std::endl;

    std::cout << "Peak High: " << FT->peakHigh << std::endl;
    std::cout << "Peak Low: " << FT->peakLow << std::endl;
    std::cout << "Peaks: \n\t";
    for (int i = 0; i < FT->getK(); i++) { std::cout << FT->peaks->at(i).at(0) << " : " << FT->peaks->at(i).at(1) << ", \n\t"; }
    std::cout << std::endl;

    std::vector<double> * NormalizedFrequencies = FT->getNormalizedFrequency();
    std::cout << "Normalized Peaks: \n\t";
    for (int i = 0; i < FT->getK(); i++) { std::cout << FT->NormalizedPeaks->at(i).at(0) << " : " << FT->NormalizedPeaks->at(i).at(1) << "%, \n\t"; }
    std::cout << std::endl;


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


        sf::Vertex line[2];
        line[0].position = sf::Vector2f(0, baseLine);
        line[0].color  = sf::Color::White;
        line[1].position = sf::Vector2f(1000, baseLine);
        line[1].color = sf::Color::White;
        app.draw(line, 2, sf::Lines);

        for (int i = 0; i < 1000; i += 10) {
            sf::Vertex l[2];
            l[0].position = sf::Vector2f(i, 0);
            l[0].color  = sf::Color::Cyan;
            l[1].position = sf::Vector2f(i, 1000);
            l[1].color = sf::Color::Cyan;
            app.draw(l, 2, sf::Lines);


            sf::Text t;
            t.setFont(f);
            t.move(i,baseLine2 - 100);
            t.setString(std::to_string(i/10));
            t.setCharacterSize(8);
            t.setFillColor(sf::Color::White);
            app.draw(t);
        }

        for (int i = 0; i < sizeof(baseLines) / sizeof(baseLines[0]); i++) {
            sf::Vertex line2[2];
            line2[0].position = sf::Vector2f(0, baseLines[i]);
            line2[0].color  = sf::Color::White;
            line2[1].position = sf::Vector2f(1000, baseLines[i]);
            line2[1].color = sf::Color::White;
            app.draw(line2, 2, sf::Lines);
        }

        for (int i = 0; i < NormalizedFrequencies->size(); i++) {
            sf::CircleShape c(2); c.setFillColor(sf::Color::Green);
            c.setPosition(i, baseLineN2 - NormalizedFrequencies->at(i));
            app.draw(c);
        }

        //std::cout << "Size = " << FT->getOriginalWaveSize() << std::endl;
        if (kilo) {
            for (int i = 0; i < FT->getOriginalWaveSize(); i++)
            {

                //std::cout << i << ": " << FT->FrequencyOutput->at(i).real() << std::endl;

                sf::CircleShape c(5); c.setFillColor(sf::Color::Red);
                c.setPosition(i * 10, baseLine2 + FT->FrequencyOutput->at(i).real() * (0.1));
                app.draw(c);
            }
        }else {
            std::cout << "Fuck" << std::endl;
            /*for (int i = 0; i < sizeof(test) / sizeof(test[0]); i++)
            {

                std::cout << i << ": " << data[i] << std::endl;

                sf::CircleShape c(5); c.setFillColor(sf::Color::Red);
                c.setPosition(i * 10, baseLine2 + data[i].real() * (1));
                app.draw(c);
            }*/
        }

        for (int i = 0; i < FT->getOriginalWaveSize(); i++) {
            sf::CircleShape c(3); c.setFillColor(sf::Color::Blue);
            c.setPosition(i, baseLine + FT->ActualValues->at(i) * (dstMult));
            app.draw(c);
        }

        /*// inverse fft
        ifft(data);

        std::cout << std::endl << "ifft" << std::endl;
        for (int i = 0; i < 8; ++i)
        {
            std::cout << data[i] << std::endl;
        }*/

        // Update the window
        app.display();
    }

    return EXIT_SUCCESS;
}
