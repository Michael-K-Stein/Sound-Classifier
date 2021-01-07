#include "Graphic.h"


template<class T>
Graphic<T>::Graphic()
{
}

template<class T>
Graphic<T>::~Graphic()
{
}

template<class T>
void Graphic<T>::drawImages(data<T> * query, data<T> * input) {
	sf::RenderWindow gameWindow(sf::VideoMode(104, 84), "Images");
	gameWindow.setSize(sf::Vector2u(860, 760));

	sf::Image qImage;
	qImage.create(28, 28);
	for (int i = 0; i < query->get_feature_vector_size(); i++) {
		int c = query->get_feature_vector()->at(i);
		qImage.setPixel(i % 28, (i - (i % 28)) / 28, sf::Color(c, c, c, c));
	}
	qImage.saveToFile("qImage.png");
	sf::Texture qTexture;
	qTexture.loadFromImage(qImage);
	sf::Sprite qSprite(qTexture);
	qSprite.setPosition(100.0, 100.0);


	sf::Image iImage;
	iImage.create(28, 28);
	int x = 0;
	int y = 0;
	for (int i = 0; i < input->get_feature_vector_size(); i++) {
		int c = input->get_feature_vector()->at(i);
		iImage.setPixel(x, y, sf::Color(c, 0, 0, 255));
		x++;
		if (x == 28) { y++; x = 0; }
	}
	sf::Texture iTexture;
	iTexture.loadFromImage(iImage);
	sf::Sprite iSprite(iTexture);
	qSprite.setPosition(480.0, 500.0);

	while (gameWindow.isOpen()) {
		//gameWindow.draw(qSprite);
		gameWindow.draw(iSprite);

		gameWindow.display();
	}

	printf("Loaded Images");
}

template<class T>
void Graphic<T>::drawImage(data<T> * image, int * label) {
	sf::RenderWindow gameWindow(sf::VideoMode(200, 200), "Images");
	gameWindow.setSize(sf::Vector2u(200, 200));

	sf::Image qImage;
	qImage.create(28, 28);
	for (int i = 0; i < image->get_feature_vector_size(); i++) {
		int c = image->get_feature_vector()->at(i);
		qImage.setPixel(i % 28, (i - (i % 28)) / 28, sf::Color(c, c, c, 255));
	}
	//qImage.saveToFile("qImage.png");
	sf::Texture qTexture;
	qTexture.loadFromImage(qImage);
	sf::Sprite qSprite(qTexture);
	qSprite.setPosition(0, 0);
	qSprite.setScale(sf::Vector2f(5, 5));

	sf::Text tLabel;
	tLabel.setString("Prediction: " + std::string((char *)(label)));
	tLabel.setPosition(20, 150);
	tLabel.setFillColor(sf::Color::White);
	sf::Font f = sf::Font();
	f.loadFromFile("Fonts/arial.ttf");
	tLabel.setFont(f);

	gameWindow.draw(qSprite);
	gameWindow.draw(tLabel);


	gameWindow.display();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

}