#include "partical.h"
#include "world.h"

Partical::Partical(float x, float y, float width, float height, float scale, string sprite, World* p)
{
	this->scale = scale;
	this->x = x;
	this->y = y;
	this->width = width * scale;
	this->height = height * scale;

	char *cstr = &sprite[0u];
	textures[8];

	timeSinceFrameChange = 0.0;
	delayBetweenFrameChanges = 0.04;

	currentSprite = 0;
	maxSprites = 6;
	parent = p;


	for (int i = 0; i < maxSprites; i++)
	{
		string imgSrc = "particals/" + sprite + "/" + to_string(i) + ".png";
		char *cstr = &imgSrc[0u];
		textures[i] = App::loadPNG(cstr);
	}
}

void Partical::update()
{
	incrementSpriteCounter();
	if (currentSprite == maxSprites)
	{
		destroyOnEnd();
	}
	timeSinceFrameChange += App::deltaTime;
}

void Partical::incrementSpriteCounter()
{
	if (timeSinceFrameChange > delayBetweenFrameChanges)
	{
		currentSprite += 1;
		timeSinceFrameChange = 0.0;
	}
}

void Partical::destroyOnEnd()
{
	parent->particals.erase(std::find(parent->particals.begin(), parent->particals.end(), *this));
}

bool Partical::operator==(const Partical & p)
{
	if (ID == p.ID)
	{
		return true;
	}
	return false;
}

void Partical::display()
{
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, textures[currentSprite]);
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