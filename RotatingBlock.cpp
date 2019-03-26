#include "rotatingBlock.h"

RotatingBlock::RotatingBlock(float x, float y, float width, float height, float spriteWidth, float spriteHeight, float rotation, float friction, string sprite)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->spriteWidth = width / spriteWidth;
	this->spriteHeight = height / spriteHeight;
	this->rotation = rotation;
	this->friction = friction;

	p0 = App::Point{ 0.0, 0.0 };
	p1 = App::Point{ 0.0, 0.0 };
	p2 = App::Point{ 0.0, 0.0 };
	p3 = App::Point{ 0.0, 0.0 };

	calculatePoints();

	char *cstr = &sprite[0u];
	texture = App::loadPNG(cstr, true, true);
}

void RotatingBlock::calculatePoints()
{
	float r = rotation * App::degToRad;
	p0.x = x + ((-width / 2)  * cos(r)) - ((height / 2) * sin(r));
	p0.y = y + ((-width / 2)  * sin(r)) + ((height / 2) * cos(r));
	p1.x = x + ((width / 2)  * cos(r)) - ((height / 2) * sin(r));
	p1.y = y + ((width / 2)  * sin(r)) + ((height / 2) * cos(r));
	p2.x = x + ((width / 2)  * cos(r)) - ((-height / 2) * sin(r));
	p2.y = y + ((width / 2)  * sin(r)) + ((-height / 2) * cos(r));
	p3.x = x + ((-width / 2)  * cos(r)) - ((-height / 2) * sin(r));
	p3.y = y + ((-width / 2)  * sin(r)) + ((-height / 2) * cos(r));
}

void RotatingBlock::display()
{
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, spriteHeight);			 glVertex2f(-width / 2, height / 2);
	glTexCoord2f(spriteWidth, spriteHeight); glVertex2f(width / 2, height / 2);
	glTexCoord2f(spriteWidth, 0);			 glVertex2f(width / 2, -height / 2);
	glTexCoord2f(0, 0);						 glVertex2f(-width / 2, -height / 2);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	App::displayBoundingBox(-width/2, -height/2, width/2, height/2);
	glPopMatrix();

	/*
	glPointSize(10);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	glVertex3f(p0.x, p0.y, 0);
	glEnd();
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_POINTS);
	glVertex3f(p1.x, p1.y, 0);
	glEnd();
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_POINTS);
	glVertex3f(p2.x, p2.y, 0);
	glEnd();
	glBegin(GL_POINTS);
	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(p3.x, p3.y, 0);
	glEnd();
	glColor3f(1.0, 1.0, 1.0);*/
}