#include "DrawableItem.h"



DrawableItem::DrawableItem() {
}

DrawableItem::DrawableItem(sf::Sprite mySprite) {
	DrawableItem::mySprite = mySprite;
}

DrawableItem::DrawableItem(std::string TexturePath) {
	DrawableItem::myTexture.loadFromFile(TexturePath);
	sf::Sprite sItem(DrawableItem::myTexture);
	DrawableItem::mySprite = sItem;
}

DrawableItem::~DrawableItem() {
}

void DrawableItem::Draw(Game& g) {
	mySprite.setPosition(DrawableItem::x, DrawableItem::y);
	g.gameWindow.draw(DrawableItem::mySprite);
}

void DrawableItem::SetPosition(float x, float y) {
	DrawableItem::x = x;
	DrawableItem::y = y;
	DrawableItem::mySprite.setPosition(x, y);
	//DrawableItem::mySprite.setRotation(rotation);
}

void DrawableItem::AddPoint(float x, float y) {
	DrawableItem::x += x;
	DrawableItem::y += y;
	DrawableItem::mySprite.setPosition(DrawableItem::x, DrawableItem::y);
}