#include "staticBlock.h"

StaticBlock::StaticBlock(float x, float y, float width, float height, float spriteWidth, float spriteHeight, string sprite, bool collider, bool repeatX, bool repeatY)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->spriteWidth = width / spriteWidth;
	this->spriteHeight = height / spriteHeight;

	this->collider = collider;

	char *cstr = &sprite[0u];
	texture = App::loadPNG(cstr, repeatX, repeatY);
}

void StaticBlock::display()
{
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, spriteHeight);			 glVertex2f(0, height);
	glTexCoord2f(spriteWidth, spriteHeight); glVertex2f(width, height);
	glTexCoord2f(spriteWidth, 0);			 glVertex2f(width, 0);
	glTexCoord2f(0, 0);						 glVertex2f(0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	App::displayBoundingBox(0, 0, width, height);
	glPopMatrix();
}