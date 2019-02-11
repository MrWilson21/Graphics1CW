#include "Player.h"

Player::Player()
{
	speed = 0.0;
	x = 200.0;
	y = 200.0;

	maxSpeed = 200.0;
	walkingAcceleration = 1500.0;
	deccelerationFactor = 1000.0;
	
	xReflectFactor = 0;
	facingLeft = false;

	isJumping = false;
	isRunning = false;
	isWalking = false;
	isIdle = false;

	currentSprite = 0;
	walkingTextures[12];
	idleTextures[18];

	timeSinceFrameChange = 0.0;
	timeToWaitForNextWalkingFrame = 10.0;
	timeSinceIdleAnimation = 0.0;
	timeToWaitUntilIdleAnimation = 3.0;
	timeToWaitForNextIdleFrame = 0.03;

	scaleFactor = 3;
	walkingHeight = 68.0 * scaleFactor;
	walkingWidth = 44.0 * scaleFactor;
	idleHeight = 75.0 * scaleFactor;
	idleWidth = 36.0 * scaleFactor;
}

void Player::incrementSpriteCounter()
{
	if (isIdle)
	{
		if (currentSprite > 0)
		{
			if (timeSinceFrameChange > timeToWaitForNextIdleFrame)
			{
				currentSprite = (currentSprite + 1) % maxSprites;
				timeSinceFrameChange = 0.0;
			}
			timeSinceFrameChange += App::deltaTime;
		}
		else
		{
			timeSinceIdleAnimation += App::deltaTime;
			if (timeSinceIdleAnimation > timeToWaitUntilIdleAnimation)
			{
				currentSprite += 1;
				timeSinceFrameChange = 0.0;
			}
		}
	}

	else if (isWalking)
	{
		if (timeSinceFrameChange > timeToWaitForNextWalkingFrame)
		{
			currentSprite = (currentSprite + 1) % maxSprites;
			timeSinceFrameChange = 0.0;
		}
		timeSinceFrameChange += abs(speed) * App::deltaTime;
	}
}

void Player::loadSprites()
{
	for (int i = 0; i < 12; i++)
	{
		string imgSrc = "shaggy/walking/" + to_string(i) + ".png";
		char *cstr = &imgSrc[0u];
		walkingTextures[i] = App::loadPNG(cstr);
	}

	for (int i = 0; i < 18; i++)
	{
		string imgSrc = "shaggy/idle/" + to_string(i) + ".png";
		char *cstr = &imgSrc[0u];
		idleTextures[i] = App::loadPNG(cstr);
	}
}

void Player::updatePlayer()
{
	if (App::keys[VK_LEFT])
	{
		speed -= walkingAcceleration * App::deltaTime;
		changeToWalkingState();
		if (speed < -maxSpeed)
		{
			speed = -maxSpeed;
		}
	}
	else if (App::keys[VK_RIGHT])
	{
		speed += walkingAcceleration * App::deltaTime;
		changeToWalkingState();
		if (speed > maxSpeed)
		{
			speed = maxSpeed;
		}
	}
	//If movement keys are not pressed then begin to deccelerate
	//If speed is less than total acceleration for a frame then set speed to 0
	else 
	{
		if (speed > deccelerationFactor  * App::deltaTime)
		{
			speed -= deccelerationFactor * App::deltaTime;
		}
		else if (speed < -deccelerationFactor * App::deltaTime)
		{
			speed += deccelerationFactor * App::deltaTime;
		}
		else
		{
			changeToIdleState();
		}
	}

	//Check direction that player is moving and face shaggy in the correct position
	if (speed < 0)
	{
		facingLeft = true;
		xReflectFactor = 1;
	}
	else if (speed > 0)
	{
		facingLeft = false;
		xReflectFactor = 0;
	}

	//Check if player is not moving and change to idle
	//Final check done in case player is walking into wall
	//give shaggy an idle state so it doesnt look like he is running on the spot into a wall
	if (speed == 0)
	{
		changeToIdleState();
	}

	//Update sprite frames
	/*timeSinceFrameChange += App::deltaTime;
	if (timeSinceFrameChange > timeBetweenFrameChanges)
	{
		timeSinceFrameChange = 0.0;
		currentSprite = (currentSprite + 1) % maxSprites;
	}
	cout << App::degToRad << "\n";*/
	x += speed * App::deltaTime;
	incrementSpriteCounter();
}

void Player::changeToWalkingState()
{
	if (!isWalking)
	{
		resetStates();
		maxSprites = 12;
		isWalking = true;
	}
}
void Player::changeToRunningState()
{

}
void Player::changeToIdleState()
{
	if (!isIdle)
	{
		resetStates();
		maxSprites = 18;
		speed = 0;
		timeSinceIdleAnimation = 0.0;
		isIdle = true;
	}
}
void Player::changeToJumpingState()
{

}
void Player::resetStates()
{
	currentSprite = 0;
	isJumping = false;
	isRunning = false;
	isWalking = false;
	isIdle = false;
}

void Player::displayWalking()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, walkingTextures[currentSprite]);
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0 + xReflectFactor, 1); glVertex2f(0, walkingHeight);
	glTexCoord2f(1 - xReflectFactor, 1); glVertex2f(walkingWidth, walkingHeight);
	glTexCoord2f(1 - xReflectFactor, 0); glVertex2f(walkingWidth, 0);
	glTexCoord2f(0 + xReflectFactor, 0); glVertex2f(0, 0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void Player::displayIdle()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, idleTextures[currentSprite]);
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0 + xReflectFactor, 1); glVertex2f(0, idleHeight);
	glTexCoord2f(1 - xReflectFactor, 1); glVertex2f(idleWidth, idleHeight);
	glTexCoord2f(1 - xReflectFactor, 0); glVertex2f(idleWidth, 0);
	glTexCoord2f(0 + xReflectFactor, 0); glVertex2f(0, 0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void Player::displayPlayer()
{
	if (isWalking)
	{
		displayWalking();
		return;
	}
	else
	{
		displayIdle();
	}
}