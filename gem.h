#pragma once
#include "app.h"
#include "player.h"

class Gem
{
public:
	float x;
	float y;
	float width;
	float height;

	Gem(float x1, float y1, int orderNumber, string sprite);
	void display();
	void displayUI(float screenWidth, float screenHeight);
	void update(Player* p);

	GLuint textures[8];
	GLuint black;

	bool collected;

private:
	void collideWithPlayer(Player* p);
	void incrementSpriteCounter();

	float timeSinceFrameChange;
	float delayBetweenFrameChanges;
	int currentSprite;
	int maxSprites;

	float hoverHeight;
	float speed;
	float velocityY;
	bool movingDown;
	float maxVel;

	float scale;
	float UIScale;
	float offSetFromLeft;
	float offsetFromTop;
	float spacing;
	int orderNumber;
};