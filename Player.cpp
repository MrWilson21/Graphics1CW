#include "player.h"

Player::Player(float startX, float startY, float scale)
{
	velocityX = 0.0;
	velocityY = 0.0;
	x = startX;
	y = startY;

	maxVelocityX = 200.0;
	maxVeloctyY = 500.0;
	walkingAcceleration = 1500.0;
	deccelerationFactor = 1000.0;
	gravity = 500.0;
	airAcceleration = 400.0;
	airDeccelerationFactor = 300.0;
	maxAirVelocityX = 100.0;
	
	xReflectFactor = 0;
	facingLeft = false;

	isJumping = false;
	isRunning = false;
	isWalking = false;
	isIdle = false;

	currentSprite = 0;
	walkingTextures[12];
	idleTextures[18];

	timeToWaitForNextWalkingFrame = 10.0;
	timeToWaitUntilIdleAnimation = 3.0;
	timeToWaitForNextIdleFrame = 0.03;
	timeToWaitForNextJumpingFrame = 0.1;
	timeUntilChangeToJump = 0.1;

	//Scale player size up or down
	scaleFactor = scale;

	//Display sizes for different player models
	walkingHeight = 68.0 * scaleFactor;
	walkingWidth = 44.0 * scaleFactor;
	idleHeight = 75.0 * scaleFactor;
	idleWidth = 36.0 * scaleFactor;
	jumpingHeight = 68 * scaleFactor;
	jumpingWidth = 34 * scaleFactor;
	

	calculateColliderBox();
}

//Sizes for player collider box
//Collider sizes can be different to display size and can also be position offset
//x and y change position of bottom left corner of box
//Height and width change size of box
void Player::calculateColliderBox()
{
	if (isWalking || isIdle)
	{
		if (facingLeft)
		{
			colliderX = 6 * scaleFactor;
			colliderY = 0;
			colliderHeight = 65 * scaleFactor;
			colliderWidth = 30 * scaleFactor;
		}
		else
		{
			colliderX = 0;
			colliderY = 0;
			colliderHeight = 65 * scaleFactor;
			colliderWidth = 30 * scaleFactor;
		}
	}
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
		//Multiply velocity with time so frame changes are based on player speed as well as time elapsed
		timeSinceFrameChange += abs(velocityX) * App::deltaTime;
	}
	else if (isJumping)
	{
		if (timeSinceFrameChange > timeToWaitForNextJumpingFrame)
		{
			//Jump animation is held at frame 4 until jump lands where jump animation is finished and state can change to idle
			if (currentSprite == 8)
			{
				changeToIdleState();
				return;
			}
			if (currentSprite < 3 || jumpLanding)
			{
				currentSprite = (currentSprite + 1) % maxSprites;
				timeSinceFrameChange = 0.0;
			}
		}
		timeSinceFrameChange += App::deltaTime;
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

	for (int i = 0; i < 9; i++)
	{
		string imgSrc = "shaggy/jump/" + to_string(i) + ".png";
		char *cstr = &imgSrc[0u];
		jumpingTextures[i] = App::loadPNG(cstr);
	}
}

void Player::updatePlayer(std::vector<StaticBlock> staticBlocks)
{
	//Get player input and change shaggys position and orientation
	getMovementUpdates();

	//GetShape of collider box for this frame
	calculateColliderBox();

	//After shaggy is moved calculate collisions and make adjustments before rendering frame
	getCollisionUpdates(staticBlocks);

	//Check if player is not moving and change to idle
	//Final check done in case player is walking into wall
	//give shaggy an idle state so it doesnt look like he is running on the spot into a wall
	if (velocityX == 0 && velocityY == 0 && !isJumping)
	{
		changeToIdleState();
	}
}

//Get player input and move shaggy based on current state and inputs given
void Player::getMovementUpdates()
{
	if (isJumping)
	{
		airMovementUpdate();
	}
	else
	{
		groundMovementUpdate();
	}
}

void Player::groundMovementUpdate()
{
	groundMove();

	if (App::keys[VK_SPACE])
	{
		velocityY = 500;
		changeToJumpingState();
	}
	else if (abs(velocityX) > 0)
	{
		changeToWalkingState();
	}

	velocityY -= gravity * App::deltaTime;
	if (velocityY < -maxVeloctyY)
	{
		velocityY = -maxVeloctyY;
	}

	//Check direction that player is moving and face shaggy in the correct position
	if (velocityX < 0)
	{
		facingLeft = true;
		xReflectFactor = 1;
	}
	else if (velocityX > 0)
	{
		facingLeft = false;
		xReflectFactor = 0;
	}

	xMoveThisFrame = velocityX * App::deltaTime;
	yMoveThisFrame = velocityY * App::deltaTime;

	x += xMoveThisFrame;
	y += yMoveThisFrame;

	incrementSpriteCounter(); 
}

void Player::airMovementUpdate()
{
	if (jumpLanding)
	{
		groundMove();
	}
	else
	{
		airMove();
	}

	velocityY -= gravity * App::deltaTime;
	if (velocityY < -maxVeloctyY)
	{
		velocityY = -maxVeloctyY;
	}

	//Check direction that player is moving and face shaggy in the correct position
	if (velocityX < 0)
	{
		facingLeft = true;
		xReflectFactor = 1;
	}
	else if (velocityX > 0)
	{
		facingLeft = false;
		xReflectFactor = 0;
	}

	xMoveThisFrame = velocityX * App::deltaTime;
	yMoveThisFrame = velocityY * App::deltaTime;

	x += xMoveThisFrame;
	y += yMoveThisFrame;

	incrementSpriteCounter();
}

void Player::groundMove()
{
	if (App::keys[VK_LEFT])
	{
		velocityX -= walkingAcceleration * App::deltaTime;
		if (velocityX < -maxVelocityX)
		{
			velocityX = -maxVelocityX;
		}
	}
	else if (App::keys[VK_RIGHT])
	{
		velocityX += walkingAcceleration * App::deltaTime;
		if (velocityX > maxVelocityX)
		{
			velocityX = maxVelocityX;
		}
	}
	//If movement keys are not pressed then begin to deccelerate
	//If speed is less than total acceleration for a frame then set speed to 0
	else
	{
		if (velocityX > deccelerationFactor  * App::deltaTime)
		{
			velocityX -= deccelerationFactor * App::deltaTime;
		}
		else if (velocityX < -deccelerationFactor * App::deltaTime)
		{
			velocityX += deccelerationFactor * App::deltaTime;
		}
		else
		{
			velocityX = 0;
		}
	}
}

void Player::airMove()
{
	if (velocityX > maxAirVelocityX)
	{
		if (velocityX > airDeccelerationFactor  * App::deltaTime)
		{
			velocityX -= airDeccelerationFactor * App::deltaTime;
		}
		else if (velocityX < -airDeccelerationFactor * App::deltaTime)
		{
			velocityX += airDeccelerationFactor * App::deltaTime;
		}
		else
		{
			velocityX = 0;
		}
	}
	else
	{
		if (App::keys[VK_LEFT])
		{
			velocityX -= airAcceleration * App::deltaTime;
			if (velocityX < -maxAirVelocityX)
			{
				velocityX = -maxAirVelocityX;
			}
		}
		else if (App::keys[VK_RIGHT])
		{
			velocityX += airAcceleration * App::deltaTime;
			if (velocityX > maxAirVelocityX)
			{
				velocityX = maxAirVelocityX;
			}
		}
		//If movement keys are not pressed then begin to deccelerate
		//If speed is less than total acceleration for a frame then set speed to 0
		else
		{
			if (velocityX > airDeccelerationFactor  * App::deltaTime)
			{
				velocityX -= airDeccelerationFactor * App::deltaTime;
			}
			else if (velocityX < -airDeccelerationFactor * App::deltaTime)
			{
				velocityX += airDeccelerationFactor * App::deltaTime;
			}
			else
			{
				velocityX = 0;
			}
		}
	}
}

void Player::getCollisionUpdates(std::vector<StaticBlock> staticBlocks)
{
	//Assume player is not touching ground until a ground collision is made
	//Set player state to jumping if no ground is collided with
	bool isTouchingGround = false;

	for (StaticBlock block : staticBlocks)
	{
		if (x + colliderX < block.width + block.x &&
			x + colliderX + colliderWidth > block.x &&
			y + colliderY < block.height + block.y &&
			y + colliderY + colliderHeight > block.y)
		{
			//Find closest edge and place player outside that edge
			//Set velocity of y to 0 if horizontal edge found and x to 0 if vertical edge found
			float playerRightSide = x + colliderX + colliderWidth;
			float playerLeftSide = x + colliderX;
			float playerTopSide = y + colliderY + colliderHeight;
			float playerBottomSide = y + colliderY;
			
			//Get distance away from each side
			//Biggest distance would have the shortest travel to end the collision
			float distToRight = block.x + block.width - playerLeftSide + xMoveThisFrame;
			float distToLeft = playerRightSide - block.x - xMoveThisFrame;
			float distToTop = block.y + block.height - playerBottomSide + yMoveThisFrame;
			float distToBottom = playerTopSide - block.y - yMoveThisFrame;
			
			//Choose the shortest travel and move player outside of collider
			if (distToRight < distToLeft &&
				distToRight < distToTop &&
				distToRight < distToBottom )
			{
				x = block.x + block.width - colliderX;
				if (velocityX < 0.0)
				{
					velocityX = 0.0;
				}
			}
			else if (distToLeft < distToRight &&
				distToLeft < distToTop &&
				distToLeft < distToBottom)
			{
				x = block.x - colliderWidth - colliderX;
				if (velocityX > 0.0)
				{
					velocityX = 0.0;
				}
			}
			else if (distToBottom < distToLeft &&
				distToBottom < distToTop &&
				distToBottom < distToRight)
			{
				y = block.y - colliderHeight - colliderY;
				if (velocityY > 0.0)
				{
					velocityY = 0.0;
				}
			}
			//If player is shortest to top or perfectly in the middle of object then push out to the top
			else
			{
				y = block.y + block.height - colliderY;
				if (velocityY < 0.0)
				{
					velocityY = 0.0;
				}
				isTouchingGround = true;
			}
		}
	}
	if (!isTouchingGround && !isJumping)
	{
		//Short delay needed as some very short frames may not detect any collisions even if player is on a platform
		//Delay also smoothes out animation slightly
		timeSinceNotTouchingGround += App::deltaTime;
		if (timeSinceNotTouchingGround > timeUntilChangeToJump)
		{
			changeToJumpingState();
			timeSinceNotTouchingGround = 0.0;
		}
	}
	if (isJumping && isTouchingGround)
	{
		jumpLanding = true;
	}
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
		velocityX = 0;
		timeSinceIdleAnimation = 0.0;
		isIdle = true;
	}
}
void Player::changeToJumpingState()
{
	if (!isJumping)
	{
		resetStates();
		maxSprites = 9;
		jumpLanding = false;
		isJumping = true;
	}
}
void Player::resetStates()
{
	currentSprite = 0;
	timeSinceFrameChange = 0.0;
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
	glDisable(GL_TEXTURE_2D);
	App::displayBoundingBox(colliderX, colliderY, colliderX + colliderWidth, colliderY + colliderHeight);
	glPopMatrix();
}

void Player::displayJumping()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, jumpingTextures[currentSprite]);
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0 + xReflectFactor, 1); glVertex2f(0, jumpingHeight);
	glTexCoord2f(1 - xReflectFactor, 1); glVertex2f(jumpingWidth, jumpingHeight);
	glTexCoord2f(1 - xReflectFactor, 0); glVertex2f(jumpingWidth, 0);
	glTexCoord2f(0 + xReflectFactor, 0); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	App::displayBoundingBox(colliderX, colliderY, colliderX + colliderWidth, colliderY + colliderHeight);
	glPopMatrix();
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
	glDisable(GL_TEXTURE_2D);
	App::displayBoundingBox(colliderX, colliderY, colliderX + colliderWidth, colliderY + colliderHeight);
	glPopMatrix();
}

void Player::displayPlayer()
{
	if (isWalking)
	{
		displayWalking();
		return;
	}
	else if(isJumping)
	{
		displayJumping();
	}
	else
	{
		displayIdle();
	}
}