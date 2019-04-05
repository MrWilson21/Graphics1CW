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
	if (App::mouseX > x * screenWidth - width / 2.0 &&
		App::mouseX < x * screenWidth + width / 2.0 &&
		App::mouseY > y * screenHeight - height / 2.0 &&
		App::mouseY < y * screenHeight + height / 2.0)
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
	glTranslatef(screenWidth * x - width / 2.0, screenHeight * y -height / 2.0, 0.0);
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

	glPushMatrix();
	int charWidth = App::screenWidthPixels * App::fontSize / screenWidth - 3;
	int noOfChars = text.length();
	float paddingX = ((App::screenWidthPixels / App::screenWidth) * width - noOfChars * charWidth) / 2;
	if (paddingX < 0)
	{
		paddingX = 0;
	}
	float paddingY = ((App::screenHeightPixels / App::screenHeight) * height - charWidth) / 2;
	glTranslatef((App::screenWidthPixels / App::screenWidth) * (screenWidth * x - width / 2.0) + paddingX, (App::screenHeightPixels / App::screenHeight) * (screenHeight * y - height / 2.0) + paddingY, 0.0);
	print(App::gameFont, 0, 0, &text[0u]);
	glPopMatrix();
}

