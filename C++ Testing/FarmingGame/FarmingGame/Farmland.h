#pragma once
#include <SFML/Graphics.hpp>
#include "GameItem.h"
class Farmland :
	public GameItem
{
public:
	Farmland(int x, int y);
	~Farmland();
	bool CheckClicked(sf::RenderWindow& gameWindow);
	void SetObject(GameItem itemType);
	void Draw(sf::RenderWindow& gameWindow);
	bool isFull();
	void SetTicksToGrow(int inpTicks);
	void Grow();
	GameItem itemType;
	int getCropAmount();
	void CropsRemoved();
	int CoolOffTime = 0;
private:
	sf::Texture objectTexture;
	int cropAmount = 0;
	int ticksToGrow = -1; // 10 Ticks per second
	int ticksPast = 0;
};

