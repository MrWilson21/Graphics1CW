#pragma once
#include "app.h"

class StaticBlock
{
public:
	float x1;
	float y1;
	float x2;
	float y2;

	GLuint texture;

	StaticBlock(float x1, float y1, float x2, float y2, string sprite);
	void display();
};