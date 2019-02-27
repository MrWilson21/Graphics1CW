#pragma once
#include "app.h"

class MovingBlock
{
public:
	float x;
	float y;
	float width;
	float height;

	float xMoveThisFrame;
	float yMoveThisFrame;

	GLuint texture;

	MovingBlock(std::vector<App::Point> points, float speed, float width, float height, float spriteWidth, float spriteHeight, string sprite);
	void display();
	void move();

private:
	float spriteWidth;
	float spriteHeight;
	unsigned int pointSize;
	int currentPoint;
	float moveSpeed;
	std::vector<App::Point> points;
};