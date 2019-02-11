#include "Player.h"

Player::Player()
{
	speed = 0.0;
	x = 200.0;
	y = 200.0;

	maxSpeed = 0.2;
	walkingAcceleration = 0.0015;
	facingLeft = false;
	isMoving = false;
	currentSprite = 0;
	walkingTextures[12];
	playerTexture = 0;
	timeBetweenFrameChanges = 0.1;
	timeSinceFrameChange = 0.0;
	
	playerTexture = 0;
}

void Player::loadSprites()
{
	for (int i = 1; i < 13; i++)
	{
		string imgSrc = "shaggy/" + to_string(i) + ".png";
		char *cstr = &imgSrc[0u];
		walkingTextures[i-1] = App::loadPNG(cstr);
	}

	playerTexture = App::loadPNG("Picture1.png");
}

void Player::updatePlayer()
{
	if (App::keys[VK_LEFT])
	{
		speed -= walkingAcceleration;
		if (speed < -maxSpeed)
		{
			speed = -maxSpeed;
		}
	}
	else if (App::keys[VK_RIGHT])
	{
		speed += walkingAcceleration;
		if (speed > maxSpeed)
		{
			speed = maxSpeed;
		}
	}
	else 
	{
		if (speed > 0)
		{
			speed -= walkingAcceleration;
		}
		else if (speed < 0)
		{
			speed += walkingAcceleration;
		}
		else
		{
			speed = 0;
		}
	}

	if (speed < 0.0)
	{
		facingLeft = true;
	}
	else if (speed > 0.0)
	{
		facingLeft = false;
	}

	timeSinceFrameChange += App::deltaTime;
	if (timeSinceFrameChange > timeBetweenFrameChanges)
	{
		timeSinceFrameChange = 0.0;
		currentSprite = (currentSprite + 1) % 12;
	}

	x += speed;
}

void Player::displayPlayer()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, walkingTextures[currentSprite]);
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(-100, 100);
	glTexCoord2f(1, 1); glVertex2f(100, 100);
	glTexCoord2f(1, 0); glVertex2f(100, -100);
	glTexCoord2f(0, 0); glVertex2f(-100, -100);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}