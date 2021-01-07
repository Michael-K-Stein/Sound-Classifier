#include "data.h";

#include <thread>

#include <SFML/Graphics.hpp>

#pragma once
template<class T>
class Graphic
{
public:
	Graphic();
	~Graphic();

	static void drawImages(data<T> * query, data<T> * input);
	static void drawImage(data<T> * image, int * label);
};

