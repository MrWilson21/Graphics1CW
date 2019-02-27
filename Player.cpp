#include "player.h"
#include "world.h"

Player::Player(float startX, float startY, World* p)
{
	velocityX = 0.0;
	velocityY = 0.0;
	x = startX;
	y = startY;

	maxVelocityX = 60.0;
	maxVeloctyY = 200;
	walkingAcceleration = 300.0;
	deccelerationFactor = 200.0;
	jumpSpeed = 200;
	gravity = 350;
	airAcceleration = 140.0;
	airDeccelerationFactor = 70.0;
	maxAirVelocityX = 45.0;

	xReflectFactor = 0;
	facingLeft = false;

	currentSprite = 0;
	maxSprites = 0;
	walkingTextures[12];
	idleTextures[18];
	jumpingTextures[9];
	attackPowerTextures[10];
	attackNoPowerTextures[10];

	timeSinceFrameChange = 0.0;
	timeSinceIdleAnimation = 0.0;
	timeToWaitForNextWalkingFrame = 2.4;
	timeToWaitUntilIdleAnimation = 3.0;
	timeToWaitForNextIdleFrame = 0.06;
	timeToWaitForNextJumpingFrame = 0.04;

	timeSinceNotTouchingGround = 0.0;
	timeUntilChangeToJump = 0.01;
	jumpLanding = false;

	gemsCollected = 0;
	health = 12;

	//Display sizes for different player models, sizes based off of sprite image size
	scaleFactor = 0.2;
	walkingHeight = 68.0 * scaleFactor;
	walkingWidth = 44.0 * scaleFactor;
	idleHeight = 75.0 * scaleFactor;
	idleWidth = 36.0 * scaleFactor;
	jumpingHeight = 68 * scaleFactor;
	jumpingWidth = 34 * scaleFactor;
	attackingHeight = 68 * scaleFactor;
	attackingWidth = 42 * scaleFactor;

	fistScale = 0.15;
	fistWidth = 936 * fistScale * scaleFactor;
	fistHeight = 672 * fistScale * scaleFactor;
	fistColliderWidth = 10;
	fistColliderHeight = fistHeight;
	fistVelocityX = 250.0;
	fistActive = false;
	fistDistanceToMove = 50.0;
	fistDistanceBeforeFadingOut = 30.0;
	fistXOffset = 5;
	fistColliderActive = false;
	gemsNeededToAttack = 6;
	timeToWaitForNextAttackingFrame = 0.05;
	fistFadingIn = false;
	fistFadingOut = false;
	fistFadeInSpeed = 3.5;
	fistFadeOutSpeed = 1.0;

	timeSinceAttack = 0.0;
	delayBetweenAttacks = 0.1;
	
	heartScale = 0.4;
	heartOffsetFromTop = 5;
	heartOffsetFromRight = 5;
	heartSpacing = 1.5;
	heartWidth = 15 * heartScale;
	heartHeight = 13 * heartScale;

	parent = p;

	resetStates();
	changeToIdleState();
	calculateColliderBox();
}

//Sizes for player collider box
//Collider sizes can be different to display size and can also be position offset
//x and y change position of bottom left corner of box
//Height and width change size of box
void Player::calculateColliderBox()
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
	else if (isAttacking)
	{
		if (timeSinceFrameChange > timeToWaitForNextAttackingFrame)
		{
			currentSprite = currentSprite + 1;
			timeSinceFrameChange = 0.0;
			if (currentSprite == maxSprites)
			{
				timeSinceAttack = 0.0;
				changeToIdleState();
			}
		}
		//Multiply velocity with time so frame changes are based on player speed as well as time elapsed
		timeSinceFrameChange += App::deltaTime;
	}
	else
	{
		cout << "sprite increment error, no state detected\n";
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

	for (int i = 0; i < 10; i++)
	{
		string imgSrc = "shaggy/attack/power/" + to_string(i) + ".png";
		char *cstr = &imgSrc[0u];
		attackPowerTextures[i] = App::loadPNG(cstr);
	}
	
	for (int i = 0; i < 10; i++)
	{
		string imgSrc = "shaggy/attack/noPower/" + to_string(i) + ".png";
		char *cstr = &imgSrc[0u];
		attackNoPowerTextures[i] = App::loadPNG(cstr);
	}

	for (int i = 0; i < 4; i++)
	{
		string imgSrc = "shaggy/hearts/" + to_string(i) + ".png";
		char *cstr = &imgSrc[0u];
		heartTextures[i] = App::loadPNG(cstr);
	}
	fistTexture = App::loadPNG("fist/fist.png");
}

void Player::updateFist()
{
	if (fistFadingIn)
	{
		fistTransparency += fistFadeInSpeed * App::deltaTime;
		if (fistTransparency > 1.0)
		{
			fistFadingIn = false;
			fistColliderActive = true;
			fistTransparency = 1.0;
		}
	}
	else if (fistColliderActive)
	{
		if (fistFacingLeft)
		{
			fistX -= fistVelocityX * App::deltaTime;
		}
		else
		{
			fistX += fistVelocityX * App::deltaTime;
		}
		fistDistanceMoved += fistVelocityX * App::deltaTime;
		
		if (fistDistanceMoved > fistDistanceBeforeFadingOut)
		{
			fistFadingOut = true;
			if (fistDistanceMoved > fistDistanceToMove)
			{
				fistColliderActive = false;
			}
		}
	}
	
	if (fistFadingOut)
	{
		fistTransparency -= fistFadeOutSpeed * App::deltaTime;
		if (fistTransparency < 0.0)
		{
			fistFadingOut = false;
			fistActive = false;
			fistTransparency = 0.0;
		}
	}
}

void Player::update()
{
	//Change to attack state if attack input detected
	if (!isAttacking)
	{
		if (App::keys[VK_SPACE] && timeSinceAttack > delayBetweenAttacks)
		{
			changeToAttackingState();
		}
		else if(!fistActive)
		{
			timeSinceAttack += App::deltaTime;
		}	
	}

	//Get player input and change players position and orientation
	getMovementUpdates();

	//GetShape of collider box for this frame
	calculateColliderBox();

	//After player is moved calculate collisions and make adjustments before rendering frame
	getCollisionUpdates();

	//Check if player is not moving and change to idle
	//Final check done in case player is walking into wall
	//give player an idle state so it doesnt look like he is running on the spot into a wall
	if (velocityX == 0 && velocityY == 0 && !isJumping && !isAttacking)
	{
		changeToIdleState();
	}

	if (fistActive)
	{
		updateFist();
		if (fistColliderActive)
		{
			if (fistFacingLeft)
			{
				fistColliderX = 0;
				fistColliderY = 0;
			}
			else
			{
				fistColliderX = fistWidth - 10;
				fistColliderY = 0;
			}
			collideFistWithEnemies();
		}
	}
}

//Get player input and move player based on current state and inputs given
void Player::getMovementUpdates()
{
	if (isJumping || (isAttacking && !isTouchingGround))
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

	if (App::keys[VK_UP] && !isAttacking)
	{
		velocityY = jumpSpeed;
		changeToJumpingState();
	}
	else if (abs(velocityX) > 0 && !isAttacking)
	{
		changeToWalkingState();
	}

	velocityY -= gravity * App::deltaTime;
	if (velocityY < -maxVeloctyY)
	{
		velocityY = -maxVeloctyY;
	}

	//Check direction that player is moving and face in the correct position
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

	//Check direction that player is moving and face in the correct position
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
	if (App::keys[VK_LEFT] && !isAttacking)
	{
		velocityX -= walkingAcceleration * App::deltaTime;
		if (velocityX < -maxVelocityX)
		{
			velocityX = -maxVelocityX;
		}
	}
	else if (App::keys[VK_RIGHT] && !isAttacking)
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
	if (abs(velocityX) > maxAirVelocityX)
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
		if (App::keys[VK_LEFT] && !isAttacking)
		{
			velocityX -= airAcceleration * App::deltaTime;
			if (velocityX < -maxAirVelocityX)
			{
				velocityX = -maxAirVelocityX;
			}
		}
		else if (App::keys[VK_RIGHT] && !isAttacking)
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

void Player::calculateCollider(float blockX, float blockY, float blockWidth, float blockHeight, float xMove, float yMove)
{
	//yMove and xMove a extra parameters given for moving block collisions
	//yMove gives the block collider a top edge from its previous frame
	//This allows the player to stick to platform if waiting ontop of it while the block is moving downwards
	//xMove moves player by the same x that the block moved by if standing on top of it

	if (x + colliderX < blockWidth + blockX &&
		x + colliderX + colliderWidth > blockX &&
		y + colliderY < blockHeight + blockY - yMove &&
		y + colliderY + colliderHeight > blockY)
	{
		//Find closest edge and place player outside that edge
		//Set velocity of y to 0 if horizontal edge found and x to 0 if vertical edge found
		float playerRightSide = x + colliderX + colliderWidth;
		float playerLeftSide = x + colliderX;
		float playerTopSide = y + colliderY + colliderHeight;
		float playerBottomSide = y + colliderY;

		//Get distance away from each side
		float distToRight = blockX + blockWidth - playerLeftSide;
		float distToLeft = playerRightSide - blockX;
		float distToTop = blockY + blockHeight - playerBottomSide;
		float distToBottom = playerTopSide - blockY;

		//Choose the shortest travel and move player outside of collider
		if (distToRight < distToLeft &&
			distToRight < distToTop &&
			distToRight < distToBottom)
		{
			x = blockX + blockWidth - colliderX;
			if (velocityX < 0.0)
			{
				velocityX = 0.0;
			}
		}
		else if (distToLeft < distToRight &&
			distToLeft < distToTop &&
			distToLeft < distToBottom)
		{
			x = blockX - colliderWidth - colliderX;
			if (velocityX > 0.0)
			{
				velocityX = 0.0;
			}
		}
		else if (distToBottom < distToLeft &&
			distToBottom < distToTop &&
			distToBottom < distToRight)
		{
			y = blockY - colliderHeight - colliderY;
			if (velocityY > 0.0)
			{
				velocityY = 0.0;
			}
		}
		//If player is shortest to top or perfectly in the middle of object then push out to the top
		else
		{
			//Add blocks xMovement to player position so player follows horizontal movement when standing on top of it
			x += xMove;
			y = blockY + blockHeight - colliderY;
			if (velocityY < 0.0)
			{
				velocityY = 0.0;
			}
			isTouchingGround = true;
			timeSinceNotTouchingGround = 0.0;
		}
	}
}

void Player::getCollisionUpdates()
{
	//Assume player is not touching ground until a ground collision is made
	//Set player state to jumping if no ground is collided with
	isTouchingGround = false;

	for (StaticBlock block : parent->staticBlocks)
	{
		calculateCollider(block.x, block.y, block.width, block.height, 0.0, 0.0);
	}

	for (MovingBlock block : parent->movingBlocks)
	{
		calculateCollider(block.x, block.y, block.width, block.height, block.xMoveThisFrame, block.yMoveThisFrame);
	}

	calculateCollider(parent->rightEdge, 0, 100, parent->worldSizeY, 0.0, 0.0);
	calculateCollider(-100 + parent->leftEdge, 0, 100, parent->worldSizeY, 0.0, 0.0);
	calculateCollider(0, parent->topEdge, parent->worldSizeX, 100, 0.0, 0.0);
	calculateCollider(0, -100 + parent->bottomEdge, parent->worldSizeX, 100, 0.0, 0.0);

	if (!isTouchingGround && !isJumping && !isAttacking)
	{
		//Short delay needed as some very short frames may not detect any collisions even if player is on a platform
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

void Player::collideFistWithEnemies()
{
	for (int i = 0; i < parent->enemies.size(); i++)
	{
		float eX = parent->enemies[i].colliderX + parent->enemies[i].x;
		float eY = parent->enemies[i].colliderY + parent->enemies[i].y;
		float eWidth = parent->enemies[i].colliderWidth;
		float eHeight = parent->enemies[i].colliderHeight;

		if (fistX + fistColliderX < eX + eWidth &&
			fistX + fistColliderX + fistColliderWidth > eX &&
			fistY + fistColliderY < eY + eHeight &&
			fistY + fistColliderY + fistColliderHeight > eY &&
			!parent->enemies[i].isDying)
		{
			float angle;
			if (facingLeft)
			{
				angle = 95 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (175 - (95))));
			}
			else
			{
				angle = -85 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (85 - (-85))));
			}
			parent->enemies[i].die(angle, fistFacingLeft);
		}
	}
}

void Player::changeToAttackingState()
{
	if (!isAttacking)
	{
		resetStates();
		maxSprites = 10;
		isAttacking = true;

		if (gemsCollected == gemsNeededToAttack)
		{
			if (facingLeft)
			{
				fistX = x + colliderX + (colliderWidth / 2) + fistXOffset;
			}
			else
			{
				fistX = x + colliderX + (colliderWidth / 2) - fistXOffset - fistWidth;
			}
			fistY = y + (colliderHeight / 2.0) - (fistHeight / 2.0);
			fistActive = true;
			fistColliderActive = false;
			fistFadingIn = true;
			fistFadingOut = false;
			fistDistanceMoved = 0.0;
			fistTransparency = 0.0;
			fistFacingLeft = facingLeft;
		}
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
	isAttacking = false;
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

void Player::displayAttacking()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	if (gemsCollected == gemsNeededToAttack)
	{
		glBindTexture(GL_TEXTURE_2D, attackPowerTextures[currentSprite]);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, attackNoPowerTextures[currentSprite]);
	}
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0 + xReflectFactor, 1); glVertex2f(0, attackingHeight);
	glTexCoord2f(1 - xReflectFactor, 1); glVertex2f(attackingWidth, attackingHeight);
	glTexCoord2f(1 - xReflectFactor, 0); glVertex2f(attackingWidth, 0);
	glTexCoord2f(0 + xReflectFactor, 0); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	App::displayBoundingBox(colliderX, colliderY, colliderX + colliderWidth, colliderY + colliderHeight);
	glPopMatrix();
}

void Player::displayFist()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, fistTexture);
	glPushMatrix();
	glTranslatef(fistX, fistY, 0.0);
	glColor4f(1.0, 1.0, 1.0, fistTransparency);
	glBegin(GL_POLYGON);
	glTexCoord2f(0 + fistFacingLeft, 1); glVertex2f(0, fistHeight);
	glTexCoord2f(1 - fistFacingLeft, 1); glVertex2f(fistWidth, fistHeight);
	glTexCoord2f(1 - fistFacingLeft, 0); glVertex2f(fistWidth, 0);
	glTexCoord2f(0 + fistFacingLeft, 0); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	if (fistColliderActive)
	{
		App::displayBoundingBox(fistColliderX, fistColliderY, fistColliderX + fistColliderWidth, fistColliderY + fistColliderHeight);
	}
	glPopMatrix();
}

void Player::displayHearts(int heartNo, int spriteToRender, float screenWidth, float screenHeight)
{
	float posX = screenWidth - heartOffsetFromRight - heartNo * (heartSpacing + heartWidth) - heartWidth;
	float posY = screenHeight - heartOffsetFromTop - heartHeight;

	glPushMatrix();
	glTranslatef(posX, posY, 0.0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, heartTextures[spriteToRender]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(0, heartHeight);
	glTexCoord2f(1, 1); glVertex2f(heartWidth, heartHeight);
	glTexCoord2f(1, 0); glVertex2f(heartWidth, 0);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Player::display()
{
	if (isWalking)
	{
		displayWalking();
	}
	else if(isJumping)
	{
		displayJumping();
	}
	else if (isAttacking)
	{
		displayAttacking();
	}
	else
	{
		displayIdle();
	}
	
	if (fistActive)
	{
		displayFist();
	}
}