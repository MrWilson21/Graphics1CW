#include "movingblock.h"

MovingBlock::MovingBlock(std::vector<App::Point> points, float speed, float width, float height, float spriteWidth, float spriteHeight, string sprite)
{
	App::Point p = points.front();
	x = p.x;
	y = p.y;
	this->width = width;
	this->height = height;
	this->spriteWidth = width / spriteWidth;
	this->spriteHeight = height / spriteHeight;

	char *cstr = &sprite[0u];
	texture = App::loadPNG(cstr, true, true);

	this->points = points;

	currentPoint = 1;
	pointSize = points.size();

	xMoveThisFrame = 0.0;
	yMoveThisFrame = 0.0;

	moveSpeed = speed;
}

void MovingBlock::move()
{
	float distanceY = points.at(currentPoint).y - y;
	float distanceX = points.at(currentPoint).x - x;

	float totalDistance = abs(distanceY) + abs(distanceX);

	float distanceToMoveX = 0.0;
	float distanceToMoveY = 0.0;

	if (distanceX != 0.0)
	{
		distanceToMoveX = distanceX * moveSpeed * App::deltaTime / totalDistance;
	}
	if (distanceY != 0.0)
	{
		distanceToMoveY = distanceY * moveSpeed * App::deltaTime / totalDistance;
	}
	
	if (abs(distanceX) - abs(distanceToMoveX) <= 0.0 && abs(distanceY) - abs(distanceToMoveY) <= 0.0)
	{
		xMoveThisFrame = distanceX;
		yMoveThisFrame = distanceY;
		x = points.at(currentPoint).x;
		y = points.at(currentPoint).y;
		currentPoint = (currentPoint + 1) % pointSize;
	}
	else
	{
		xMoveThisFrame = distanceToMoveX;
		yMoveThisFrame = distanceToMoveY;
		x += distanceToMoveX;
		y += distanceToMoveY;
	}
}

void MovingBlock::display()
{
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, spriteHeight); glVertex2f(0, height);
	glTexCoord2f(spriteWidth, spriteHeight); glVertex2f(width, height);
	glTexCoord2f(spriteWidth, 0); glVertex2f(width, 0);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	App::displayBoundingBox(0, 0, width, height);
	glPopMatrix();
}