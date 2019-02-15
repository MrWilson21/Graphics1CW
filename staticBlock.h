#pragma once
#include "app.h"

class StaticBlock
{
public:
	float x;
	float y;
	float width;
	float height;

	GLuint texture;

	StaticBlock(float x1, float y1, float width, float height, string sprite);
	void display();
};