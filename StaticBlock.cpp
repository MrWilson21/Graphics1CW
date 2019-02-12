#include "staticBlock.h"

StaticBlock::StaticBlock(float x1, float y1, float x2, float y2, string sprite)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;

	char *cstr = &sprite[0u];
	texture = App::loadPNG(cstr);
}

void StaticBlock::display()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPushMatrix();
	glTranslatef(x1, y1, 0.0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(0, y2);
	glTexCoord2f(1, 1); glVertex2f(x2, y2);
	glTexCoord2f(1, 0); glVertex2f(x2, 0);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}