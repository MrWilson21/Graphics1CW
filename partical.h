#pragma once
#include "app.h"

class World;
class Partical
{
public:
	float x;
	float y;
	float width;
	float height;

	Partical(float x1, float y1, float width, float height, float scale, string sprite, World* p);
	void display();
	void update();

	GLuint textures[8];
	int ID;

	bool operator==(const Partical& p);

private:
	void incrementSpriteCounter();
	void destroyOnEnd();

	float timeSinceFrameChange;
	float delayBetweenFrameChanges;
	int currentSprite;
	int maxSprites;

	float scale;

	World* parent;
};