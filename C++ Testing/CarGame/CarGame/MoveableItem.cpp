#include "MoveableItem.h"

#include <iostream>

MoveableItem::MoveableItem() {
}

MoveableItem::MoveableItem(std::string TexturePath) {
	DrawableItem::myTexture.loadFromFile(TexturePath);
	sf::Sprite sItem(DrawableItem::myTexture);
	DrawableItem::mySprite = sItem;
}

MoveableItem::~MoveableItem() {
}

void MoveableItem::MoveForward(float dist) {
	std::cout << "Deg: " << 360-DrawableItem::mySprite.getRotation() << " | " << "Cos: " << cos(Game::Deg2Rad(360-DrawableItem::mySprite.getRotation())) << std::endl;

	DrawableItem::AddPoint(cos(Game::Deg2Rad(360-DrawableItem::mySprite.getRotation())) * dist, -sin(Game::Deg2Rad(360-DrawableItem::mySprite.getRotation())) * dist);
}