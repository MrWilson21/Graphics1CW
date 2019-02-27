#include "button.h"

Button::Button()
{

}

void Button::initialise(float x, float y, float width, float height, float scale, string text, void(*function)())
{
	this->x = x;
	this->y = y;
	this->width = width * scale;
	this->height = height * scale;
	this->text = text;
	this->function = function;

	highlighted = false;

	inactiveTexture = App::loadPNG("menu/buttons/0.png");
	activeTexture = App::loadPNG("menu/buttons/1.png");
}

void Button::checkIfButtonHighlighted(float screenWidth, float screenHeight)
{
	if (App::mouseX > x * screenWidth &&
		App::mouseX < x * screenWidth + width &&
		App::mouseY > y * screenHeight &&
		App::mouseY < y * screenHeight + height)
	{
		highlighted = true;
	}
	else
	{
		highlighted = false;
	}

	if (App::leftPressed && highlighted)
	{
		function();
	}
}

void Button::display(float screenWidth, float screenHeight)
{
	glPushMatrix();
	glTranslatef(screenWidth * x, screenHeight * y, 0.0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	if (highlighted)
	{
		glBindTexture(GL_TEXTURE_2D, activeTexture);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, inactiveTexture);
	}
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(0, height);
	glTexCoord2f(1, 1); glVertex2f(width, height);
	glTexCoord2f(1, 0); glVertex2f(width, 0);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	App::displayBoundingBox(0.0, 0.0, width, height);
	glPopMatrix();
}

