#pragma once
#include "app.h"

class MovingBlock
{
public:
	float x;
	float y;
	float width;
	float height;

	GLuint texture;

	MovingBlock(std::vector<App::Point> points, float speed, float width, float height, string sprite);
	void display();
	void move();

private:
	unsigned int pointSize;
	int currentPoint;
	float moveSpeed;
	std::vector<App::Point> points;
};