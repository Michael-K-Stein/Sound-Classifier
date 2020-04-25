#include "data.h";

#include <thread>

#include <SFML/Graphics.hpp>

#pragma once
class Graphic
{
public:
	Graphic();
	~Graphic();

	static void drawImages(data * query, data * input);
	static void drawImage(data * image, int * label);
};

