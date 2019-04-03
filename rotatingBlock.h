#pragma once
#include "app.h"

class RotatingBlock
{
public:
	float x;
	float y;
	float width;
	float height;

	float friction;

	//p0 is top left corner going clockwise
	App::Point p0;
	App::Point p1;
	App::Point p2;
	App::Point p3;

	float rotation;

	GLuint texture;

	RotatingBlock(float x1, float y1, float width, float height, float spriteWidth, float spriteHeight, float rotation, string sprite = "blocks/1.png");
	void display();
	void calculatePoints();

private:
	float spriteWidth;
	float spriteHeight;
};