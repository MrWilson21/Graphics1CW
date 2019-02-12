#include "staticBlock.h"

StaticBlock::StaticBlock(float x, float y, float width, float height, string sprite)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	char *cstr = &sprite[0u];
	texture = App::loadPNG(cstr);
}

void StaticBlock::display()
{
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(0, height);
	glTexCoord2f(1, 1); glVertex2f(width, height);
	glTexCoord2f(1, 0); glVertex2f(width, 0);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	App::displayBoundingBox(0, 0, width, height);
	glPopMatrix();
}