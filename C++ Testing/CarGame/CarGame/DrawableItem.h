#pragma once
#include "Game.h"
class DrawableItem
{
public:
	DrawableItem();
	DrawableItem(sf::Sprite mySprite);
	DrawableItem(std::string TexturePath);
	~DrawableItem();
	void Draw(Game& g);
	void SetPosition(float x, float y);
	void AddPoint(float x, float y);
	sf::Texture myTexture;
	sf::Sprite mySprite;
	float x = 0;
	float y = 0;
};

