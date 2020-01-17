#pragma once
#include "Game.h"

class GameItem
{
public:
	GameItem();
	//GameItem(sf::Texture inpTexture);
	~GameItem();
	void Draw(sf::RenderWindow& rW, int ind);
	void Draw(sf::RenderWindow& rW);
	sf::Texture getTexture();
	sf::Texture itemTexture;
	virtual int getTicksToGrow();
	void SetTicksToGrow(int inpTicks);
	const char* type;
private:
protected:
	bool isInInventory;
	int IndexInInventory;
	
	struct LocationSTRUCT
	{
		int x;
		int y;
	} Location;

	int ticksToGrow = -1; // 10 Ticks per second
};

