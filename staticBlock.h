#pragma once
#include "app.h"

class StaticBlock
{
public:
	float x;
	float y;
	float width;
	float height;

	bool collider;

	GLuint texture;

	StaticBlock(float x1, float y1, float width, float height, float spriteWidth, float spriteHeight, string sprite = "blocks/1.png", bool collider = true, bool repeatX = true, bool repeatY = true);
	void display();
private:
	float spriteWidth;
	float spriteHeight;
};