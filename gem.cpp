#include "gem.h"

Gem::Gem(float x, float y, int orderNumber, string sprite)
{
	scale = 0.2;
	UIScale = 0.3;
	this->x = x;
	this->y = y;
	this->width = 22 * scale;
	this->height = 32 * scale;

	char *cstr = &sprite[0u];
	textures[8];
	collected = false;

	timeSinceFrameChange = 0.0;
	delayBetweenFrameChanges = 0.12;

	currentSprite = orderNumber;
	maxSprites = 8;

	movingDown = false;
	hoverHeight = y + 3;
	speed = 120.0;
	velocityY = 0.0;
	maxVel = 20;

	offSetFromLeft = 5;
	offsetFromTop = 5;
	spacing = 2;
	this->orderNumber = orderNumber;

	for (int i = 0; i < maxSprites; i++)
	{
		string imgSrc = "fist/gems/" + sprite + "/" + to_string(i) + ".png";
		char *cstr = &imgSrc[0u];
		textures[i] = App::loadPNG(cstr);
	}

	black = App::loadPNG("fist/gems/black.png");
}

void Gem::collideWithPlayer(Player* p)
{
	if (x < p->x + p->colliderX + p->colliderWidth &&
		x + width > p->x + p->colliderX &&
		y < p->y + p->colliderY + p->colliderHeight &&
		y + height > p->y + p->colliderY)
	{
		collected = true;
		p->createPartical(x + width/2, y + height/2, 166, 165, 0.2, "gemCollect");
		p->gemsCollected += 1;
		p->addScore(250);
	}
}

void Gem::update(Player* p)
{
	if (!collected)
	{
		if (movingDown)
		{
			if (y < hoverHeight) 
			{
				movingDown = false;
			}
			else
			{
				velocityY -= speed * App::deltaTime;
				if (velocityY < -maxVel)
				{
					velocityY = -maxVel;
				}
			}
		}
		else 
		{
			if (y > hoverHeight)
			{
				movingDown = true;
			}
			else
			{
				velocityY += speed * App::deltaTime;
				if (velocityY > maxVel)
				{
					velocityY = maxVel;
				}
			}
		}

		y += velocityY * App::deltaTime;

		collideWithPlayer(p);
	}
	incrementSpriteCounter();
}

void Gem::incrementSpriteCounter()
{
	if (timeSinceFrameChange > delayBetweenFrameChanges)
	{
		currentSprite = (currentSprite + 1) % maxSprites;
		timeSinceFrameChange = 0.0;
	}
	//Multiply velocity with time so frame changes are based on player speed as well as time elapsed
	timeSinceFrameChange += App::deltaTime;
}

void Gem::display()
{
	if (!collected)
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
}

void Gem::displayUI(float screenWidth, float screenHeight)
{
	float posX = offSetFromLeft + orderNumber * (spacing + width);
	float posY = screenHeight - offsetFromTop - height;

	glPushMatrix();
	glTranslatef(posX, posY, 0.0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	if (!collected)
	{
		glBindTexture(GL_TEXTURE_2D, black);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, textures[currentSprite]);
	}
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(0, height);
	glTexCoord2f(1, 1); glVertex2f(width, height);
	glTexCoord2f(1, 0); glVertex2f(width, 0);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}