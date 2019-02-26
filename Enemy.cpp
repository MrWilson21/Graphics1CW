#include "enemy.h"
#include "world.h"

Enemy::Enemy(float startX, float startY, World* p, int ID)
{
	this->ID = ID;

	velocityX = 0.0;
	velocityY = 0.0;
	x = startX;
	y = startY;

	maxVelocityX = 30.0;
	maxVeloctyY = 200.0;
	maxRunningVelocityX = 50.0;
	runningAcceleration = 150.0;
	walkingAcceleration = 100.0;
	deccelerationFactor = 160.0;
	gravity = 350;
	airDeccelerationFactor = 0.0;

	jumpUpHeight = 190.0;
	jumpUpXVelocity = 90.0;
	jumpDownHeight = 75.0;
	jumpDownXVelocity = 90.0;
	diveHeight = 45.0;
	diveXVelocity = 110.0;

	xReflectFactor = 0;
	facingLeft = false;
	aimingLeft = false;

	currentSprite = 0;
	maxSprites = 0;
	walkingTextures[12];
	idleTextures[2];
	diveTextures[4];
	runTextures[12];
	jumpTextures[10];

	timeSinceFrameChange = 0.0;
	timeToWaitForNextWalkingFrame = 1.2;
	timeToWaitForNextIdleFrame = 0.65;
	timeToWaitForNextJumpingFrame = 0.05;
	timeToWaitForNextRunningFrame = 1.2;
	timeToWaitForNextDivingFrame = 0.1;

	timeSinceNotTouchingGround = 0.0;
	timeUntilChangeToJump = 0.01;

	//Display sizes for different player models, sizes based off of sprite image size
	scaleFactor = 0.2;
	walkingWidth = 36.0 * scaleFactor;
	walkingHeight = 39.0 * scaleFactor;
	idleWidth = 38.0 * scaleFactor;
	idleHeight = 37.0 * scaleFactor;
	jumpingWidth = 38 * scaleFactor;
	jumpingHeight = 41 * scaleFactor;
	divingWidth = 46 * scaleFactor;
	divingHeight = 27 * scaleFactor;
	runningWidth = 37 * scaleFactor;
	runningHeight = 36 * scaleFactor;

	delayBeforeAttacking = 0.7;
	delayBetweenDives = 3.5;
	delayBetweenJumps = 1.2;
	timeSinceStartingAttack = 0.0;
	timeSinceAttacking = 0.0;
	delayAfterLandingAttack = 0.6;
	timeSinceLandingAttack = 0.0;

	dieFlySpeed = 1.0;

	parent = p;

	isTouchingRight = false;
	isTouchingLeft = false;
	isInAir = false;
	resetStates();
	changeToWalkingState();
	calculateColliderBox();
}

//Sizes for player collider box
//Collider sizes can be different to display size and can also be position offset
//x and y change position of bottom left corner of box
//Height and width change size of box
void Enemy::calculateColliderBox()
{
	chaseColliderWidth = 150;
	chaseColliderHeight = 100;
	diveColliderWidth = 45;
	diveColliderHeight = 12;
	jumpUpColliderWidth = 45;
	jumpUpColliderHeight = 20;
	jumpDownColliderWidth = 45;
	jumpDownColliderHeight = 35;

	chaseXOffset = -15;
	chaseYOffset = -5;
	diveXOffset = 30;
	diveYOffset = 0;
	jumpUpXOffset = 20;
	jumpUpYOffset = 25;
	jumpDownXOffset = 20;
	jumpDownYOffset = -20;

	colliderX = 0;
	colliderY = 0;
	colliderHeight = 38 * scaleFactor;
	colliderWidth = 37 * scaleFactor;

	float midPointX = colliderX + colliderWidth / 2;
	float midPointY = colliderY + colliderHeight / 2;

	if (!facingLeft)
	{
		chaseXOffset = chaseXOffset + midPointX;
		chaseYOffset = chaseYOffset - (chaseColliderHeight / 2) + midPointY;
		diveXOffset = diveXOffset + midPointX;
		diveYOffset = diveYOffset - (diveColliderHeight / 2) + midPointY;
		jumpUpXOffset = jumpUpXOffset + midPointX;
		jumpUpYOffset = jumpUpYOffset + midPointY;
		jumpDownXOffset = jumpDownXOffset + midPointX;
		jumpDownYOffset = jumpDownYOffset - jumpDownColliderHeight + midPointY;
	}
	else
	{
		chaseXOffset = -chaseXOffset - chaseColliderWidth + midPointX;
		chaseYOffset = chaseYOffset - (chaseColliderHeight / 2) + midPointY;
		diveXOffset = -diveXOffset - diveColliderWidth + midPointX;
		diveYOffset = diveYOffset - (diveColliderHeight / 2) + midPointY;
		jumpUpXOffset = -jumpUpXOffset - jumpUpColliderWidth + midPointX;
		jumpUpYOffset = jumpUpYOffset + midPointY;
		jumpDownXOffset = -jumpDownXOffset - jumpDownColliderWidth + midPointX;
		jumpDownYOffset = jumpDownYOffset - jumpDownColliderHeight + midPointY;
	}
}

void Enemy::incrementSpriteCounter()
{
	if (isIdle)
	{
		if (timeSinceFrameChange > timeToWaitForNextIdleFrame)
		{
			currentSprite = (currentSprite + 1) % maxSprites;
			timeSinceFrameChange = 0.0;
		}
		timeSinceFrameChange += App::deltaTime;
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
	else if (isRunning)
	{
		if (timeSinceFrameChange > timeToWaitForNextRunningFrame)
		{
			currentSprite += 1;
			if (currentSprite == maxSprites - 1)
			{
				currentSprite = 7;
			}
			timeSinceFrameChange = 0.0;
		}
		//Multiply velocity with time so frame changes are based on player speed as well as time elapsed
		timeSinceFrameChange += abs(velocityX) * App::deltaTime;
	}
	else if (isJumpingDown || isJumpingUp)
	{
		if (timeSinceFrameChange > timeToWaitForNextJumpingFrame)
		{
			//Jump animation is held at frame 1 until jump lands where jump animation is finished and state can change to idle
			if (jumpLanding)
			{
				currentSprite += 1;
				if (currentSprite == maxSprites)
				{
					currentSprite -= 1;
				}
			}
			else
			{
				return;
			}
		}
		timeSinceFrameChange += App::deltaTime;
	}
	else if (isDiving)
	{
		if (timeSinceFrameChange > timeToWaitForNextDivingFrame)
		{
			currentSprite = (currentSprite + 1) % maxSprites;
			timeSinceFrameChange = 0.0;
		}
		timeSinceFrameChange += App::deltaTime;
	}
}

void Enemy::loadSprites()
{
	for (int i = 0; i < 2; i++)
	{
		string imgSrc = "knuckles/idle/" + to_string(i) + ".png";
		char *cstr = &imgSrc[0u];
		idleTextures[i] = App::loadPNG(cstr);
	}

	for (int i = 0; i < 12; i++)
	{
		string imgSrc = "knuckles/walking/" + to_string(i) + ".png";
		char *cstr = &imgSrc[0u];
		walkingTextures[i] = App::loadPNG(cstr);
	}

	for (int i = 0; i < 12; i++)
	{
		string imgSrc = "knuckles/run/" + to_string(i) + ".png";
		char *cstr = &imgSrc[0u];
		runTextures[i] = App::loadPNG(cstr);
	}

	for (int i = 0; i < 6; i++)
	{
		string imgSrc = "knuckles/jump/" + to_string(i) + ".png";
		char *cstr = &imgSrc[0u];
		jumpTextures[i] = App::loadPNG(cstr);
	}

	for (int i = 0; i < 4; i++)
	{
		string imgSrc = "knuckles/dive/" + to_string(i) + ".png";
		char *cstr = &imgSrc[0u];
		diveTextures[i] = App::loadPNG(cstr);
	}
	waitingToAttackTexture = App::loadPNG("knuckles/waitingToAttack/0.png");
}

void Enemy::detectPlayer()
{
	float pColliderX = parent->player->colliderX + parent->player->x;
	float pColliderY = parent->player->colliderY + parent->player->y;
	float pColliderWidth = parent->player->colliderWidth;
	float pColliderHeight = parent->player->colliderHeight;

	float chaseX = x + chaseXOffset;
	float chaseY = y + chaseYOffset;

	if (chaseX < pColliderX + pColliderWidth &&
		chaseX + chaseColliderWidth > pColliderX &&
		chaseY < pColliderHeight + pColliderY &&
		chaseY + chaseColliderHeight  > pColliderY)
	{
		float playerMidPointX = pColliderX + pColliderWidth / 2;
		float enemyMidPointX = x + colliderX + colliderWidth / 2;
		if (isIdle)
		{
			if (playerMidPointX - enemyMidPointX > 0.0)
			{
				if (facingLeft)
				{
					turnRight();
					changeToRunningState();	
				}
			}
			else if (!facingLeft)
			{
				turnLeft();
				changeToRunningState();
			}
		}
		else if(isWalking || isRunning)
		{
			if (playerMidPointX - enemyMidPointX > 0.0)
			{

				turnRight();
			}
			else
			{
				turnLeft();
			}
			changeToRunningState();
		}

		if (!isWaitingToAttack && !isIdle)
		{
			float jumpUpX = x + jumpUpXOffset;
			float jumpUpY = y + jumpUpYOffset;
			float jumpDownX = x + jumpDownXOffset;
			float jumpDownY = y + jumpDownYOffset;
			float diveX = x + diveXOffset;
			float diveY = y + diveYOffset;
				
			if (jumpUpX < pColliderX + pColliderWidth &&
				jumpUpX + jumpUpColliderWidth > pColliderX &&
				jumpUpY < pColliderHeight + pColliderY &&
				jumpUpY + jumpUpColliderHeight  > pColliderY &&
				timeSinceAttacking > delayBetweenJumps)
			{
				changeToWaitingToAttack();
				isJumpingUp = true;
			}
			else if (jumpDownX < pColliderX + pColliderWidth &&
					 jumpDownX + jumpDownColliderWidth > pColliderX &&
				     jumpDownY < pColliderHeight + pColliderY &&
					 jumpDownY + jumpDownColliderHeight  > pColliderY &&
					 timeSinceAttacking > delayBetweenJumps)
			{
				changeToWaitingToAttack();
				isJumpingDown = true;
			}
			else if (diveX < pColliderX + pColliderWidth &&
					 diveX + diveColliderWidth > pColliderX &&
					 diveY < pColliderHeight + pColliderY &&
					 diveY + diveColliderHeight  > pColliderY && 
					 timeSinceAttacking > delayBetweenDives)
			{
				changeToWaitingToAttack();
				isDiving = true;
			}
		}
	}
	else if(!isWaitingToAttack)
	{
		changeToWalkingState();
	}
}

void Enemy::attack()
{
	jumpLanding = false;
	isWaitingToAttack = false;
	isAttacking = true;
	timeSinceLandingAttack = 0.0;
	timeSinceAttacking = 0.0;
	timeSinceStartingAttack = 0.0;
	if (isDiving)
	{
		if (facingLeft)
		{
			velocityX = -diveXVelocity;
			velocityY = diveHeight;
		}
		else
		{
			velocityX = diveXVelocity;
			velocityY = diveHeight;
		}
	}
	if (isJumpingDown)
	{
		if (facingLeft)
		{
			velocityX = -jumpDownXVelocity;
			velocityY = jumpDownHeight;
		}
		else
		{
			velocityX = jumpDownXVelocity;
			velocityY = jumpDownHeight;
		}
	}
	if (isJumpingUp)
	{
		if (facingLeft)
		{
			velocityX = -jumpUpXVelocity;
			velocityY = jumpUpHeight;
		}
		else
		{
			velocityX = jumpUpXVelocity;
			velocityY = jumpUpHeight;
		}
	}
}

void Enemy::update()
{
	if (!isDying)
	{
		//GetShape of collider box for this frame
		calculateColliderBox();

		//Chose behaviour if player is detected in range of enemy
		if (!isInAir && !isWaitingToAttack && !isAttacking)
		{
			detectPlayer();
		}

		if (isWaitingToAttack)
		{
			timeSinceStartingAttack += App::deltaTime;
			if (timeSinceStartingAttack > delayBeforeAttacking)
			{
				attack();
			}
		}
		else if (isAttacking && jumpLanding)
		{
			timeSinceLandingAttack += App::deltaTime;
			if (timeSinceLandingAttack > delayAfterLandingAttack)
			{
				changeToWalkingState();
			}
		}
		else if (!isAttacking)
		{
			timeSinceAttacking += App::deltaTime;
		}

		getMovementUpdates();

		isTouchingGround = false;
		isTouchingRight = false;
		isTouchingLeft = false;
		//After player is moved calculate collisions and make adjustments before rendering frame
		getCollisionUpdates();

		if (isWalkingOfLeftEdge && isWalking)
		{
			if (velocityX < 0.0 && !isInAir)
			{
				velocityX = 0.0;
				turnRight();
			}
		}
		else if (isWalkingOfRightEdge && isWalking)
		{
			if (velocityX > 0.0 && !isInAir)
			{
				velocityX = 0.0;
				turnLeft();
			}
		}

		if (isIdle)
		{
			if (!facingLeft && !isTouchingRight)
			{
				changeToWalkingState();
			}
			else if (facingLeft && !isTouchingLeft)
			{
				changeToWalkingState();
			}
		}

		incrementSpriteCounter();
	}
	else
	{
		updateDie();
	}
}

void Enemy::turnLeft()
{
	aimingLeft = true;
	if (velocityX <= 0.0)
	{
		facingLeft = true;
	}
}

void Enemy::turnRight()
{
	aimingLeft = false;
	if (velocityX >= 0.0)
	{
		facingLeft = true;
	}
}

//Get player input and move player based on current state and inputs given
void Enemy::getMovementUpdates()
{
	if (!isInAir)
	{
		groundMovementUpdate();
	}
	else
	{
		airMovementUpdate();
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
	else if (aimingLeft)
	{
		facingLeft = true;
		xReflectFactor = 1;
	}
	else
	{
		facingLeft = false;
		xReflectFactor = 0;
	}
}

void Enemy::airMovementUpdate()
{
	if (velocityX > 0.0)
	{
		if (velocityX - airDeccelerationFactor * App::deltaTime > 0.0)
		{
			velocityX -= airDeccelerationFactor * App::deltaTime > 0.0;
		}
		else
		{
			velocityX = 0.0;
		}
	}
	else
	{
		if (velocityX + airDeccelerationFactor * App::deltaTime < 0.0)
		{
			velocityX += airDeccelerationFactor * App::deltaTime > 0.0;
		}
		else
		{
			velocityX = 0.0;
		}
	}

	velocityY -= gravity * App::deltaTime;
	if (velocityY < -maxVeloctyY)
	{
		velocityY = -maxVeloctyY;
	}

	xMoveThisFrame = velocityX * App::deltaTime;
	yMoveThisFrame = velocityY * App::deltaTime;

	x += xMoveThisFrame;
	y += yMoveThisFrame;
}

void Enemy::groundMovementUpdate()
{
	if (aimingLeft)
	{
		if (isWalking)
		{
			velocityX -= walkingAcceleration * App::deltaTime;
			if (velocityX < -maxVelocityX)
			{
				velocityX = -maxVelocityX;
			}
		}
		else if(isRunning)
		{
			velocityX -= runningAcceleration * App::deltaTime;
			if (velocityX < -maxRunningVelocityX)
			{
				velocityX = -maxRunningVelocityX;
			}
		}
		else
		{
			velocityX += deccelerationFactor * App::deltaTime;
			if (velocityX > 0.0)
			{
				velocityX = 0.0;
			}
		}
	}
	else
	{
		if (isWalking)
		{
			velocityX += walkingAcceleration * App::deltaTime;
			if (velocityX > maxVelocityX)
			{
				velocityX = maxVelocityX;
			}
		}
		else if(isRunning)
		{
			velocityX += runningAcceleration * App::deltaTime;
			if (velocityX > maxRunningVelocityX)
			{
				velocityX = maxRunningVelocityX;
			}
		}
		else
		{
			velocityX -= deccelerationFactor * App::deltaTime;
			if (velocityX < 0.0)
			{
				velocityX = 0.0;
			}	
		}
	}

	velocityY -= gravity * App::deltaTime;
	if (velocityY < -maxVeloctyY)
	{
		velocityY = -maxVeloctyY;
	}

	xMoveThisFrame = velocityX * App::deltaTime;
	yMoveThisFrame = velocityY * App::deltaTime;

	x += xMoveThisFrame;
	y += yMoveThisFrame;
}

void Enemy::calculateCollider(float blockX, float blockY, float blockWidth, float blockHeight, float xMove, float yMove)
{
	//yMove and xMove a extra parameters given for moving block collisions
	//yMove gives the block collider a top edge from its previous frame
	//This allows the player to stick to platform if waiting ontop of it while the block is moving downwards
	//xMove moves player by the same x that the block moved by if standing on top of it

	if (x + colliderX <= blockWidth + blockX &&
		x + colliderX + colliderWidth >= blockX &&
		y + colliderY <= blockHeight + blockY - yMove &&
		y + colliderY + colliderHeight >= blockY)
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
			isTouchingRight = true;
			if (velocityX < 0.0)
			{
				velocityX = 0.0;
			}
			if (isWalking)
			{
				turnRight();
			}
			if (isRunning)
			{
				changeToIdleState();
			}
		}
		else if (distToLeft < distToRight &&
			distToLeft < distToTop &&
			distToLeft < distToBottom)
		{
			x = blockX - colliderWidth - colliderX;
			isTouchingLeft = true;
			if (velocityX > 0.0)
			{
				velocityX = 0.0;
			}
			if (isWalking)
			{
				turnLeft();
			}
			else if (isRunning)
			{
				changeToIdleState();
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
			//If right or left edges of enemy collider have not gone past the edge of block colliders right or left edges then enemy is not walking off an edge
			if (x + colliderX + colliderWidth / 2 >= blockX - xMove)
			{
				isWalkingOfLeftEdge = false;
			}
			if (x + colliderX + colliderWidth / 2 <= blockX + blockWidth - xMove)
			{
				isWalkingOfRightEdge = false;
			}
		}
	}
}

void Enemy::getCollisionUpdates()
{
	//Assume enemy is not touching ground until a ground collision is made
	//Also assume enemy is walking off and edge until ground is detected under each edge of enemy collider box
	isTouchingGround = false;
	isWalkingOfRightEdge = true;
	isWalkingOfLeftEdge = true;

	if ((!isAttacking))
	{
		for (Enemy e : parent->enemies)
		{
			if (this->ID != e.ID && !e.isDying)
			{
				calculateCollider(e.colliderX + e.x, e.colliderY + e.y, e.colliderWidth, e.colliderHeight, 0.0, 0.0);
			}
		}
	}

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

	if (!isTouchingGround)
	{
		//Short delay needed as some very short frames may not detect any collisions even if player is on a platform
		timeSinceNotTouchingGround += App::deltaTime;
		if (timeSinceNotTouchingGround > timeUntilChangeToJump)
		{
			isInAir = true;
		}
	}
	if ((isJumpingUp || isJumpingDown || isDiving) && isTouchingGround)
	{
		jumpLanding = true;
	}
	if (isTouchingGround)
	{
		isInAir = false;
	}
}

void Enemy::changeToWalkingState()
{
	if (!isWalking)
	{
		resetStates();
		maxSprites = 12;
		isWalking = true;
	}
}
void Enemy::changeToIdleState()
{
	if (!isIdle)
	{
		resetStates();
		maxSprites = 2;
		isIdle = true;
	}
}
void Enemy::resetStates()
{
	currentSprite = 0;
	timeSinceFrameChange = 0.0;
	isWalking = false;
	isIdle = false;
	isJumpingUp = false;
	isJumpingDown = false;
	isAttacking = false;
	isDiving = false;
	isRunning = false;
	isWaitingToAttack = false;
	jumpLanding = false;
	isDying = false;
}

void Enemy::changeToRunningState()
{
	if (!isRunning)
	{
		resetStates();
		maxSprites = 12;
		isRunning = true;
	}
}

void Enemy::changeToJumpingState()
{
	if (!isJumpingUp || !isJumpingDown)
	{
		resetStates();
		maxSprites = 10;
		isIdle = true;
	}
}

void Enemy::changeToDivingState()
{
	if (!isDiving)
	{
		resetStates();
		maxSprites = 4;
		isDiving = true;
	}
}

void Enemy::changeToWaitingToAttack()
{
	if (!isWaitingToAttack)
	{
		resetStates();
		maxSprites = 1;
		isWaitingToAttack = true;
		timeSinceStartingAttack = 0.0;
	}
}

void Enemy::displayWalking()
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
	App::displayBoundingBox(diveXOffset, diveYOffset, diveXOffset + diveColliderWidth, diveYOffset + diveColliderHeight);
	App::displayBoundingBox(chaseXOffset, chaseYOffset, chaseXOffset + chaseColliderWidth, chaseYOffset + chaseColliderHeight);
	App::displayBoundingBox(jumpDownXOffset, jumpDownYOffset, jumpDownXOffset + jumpDownColliderWidth, jumpDownYOffset + jumpDownColliderHeight);
	App::displayBoundingBox(jumpUpXOffset, jumpUpYOffset, jumpUpXOffset + jumpUpColliderWidth, jumpUpYOffset + jumpUpColliderHeight);
	glPopMatrix();
}

void Enemy::displayIdle()
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
	App::displayBoundingBox(diveXOffset, diveYOffset, diveXOffset + diveColliderWidth, diveYOffset + diveColliderHeight);
	App::displayBoundingBox(chaseXOffset, chaseYOffset, chaseXOffset + chaseColliderWidth, chaseYOffset + chaseColliderHeight);
	App::displayBoundingBox(jumpDownXOffset, jumpDownYOffset, jumpDownXOffset + jumpDownColliderWidth, jumpDownYOffset + jumpDownColliderHeight);
	App::displayBoundingBox(jumpUpXOffset, jumpUpYOffset, jumpUpXOffset + jumpUpColliderWidth, jumpUpYOffset + jumpUpColliderHeight);
	glPopMatrix();
}

void Enemy::die(float angle, bool facingLeft)
{
	changeToDieState();
	dieAngle = angle;
	this->facingLeft = facingLeft;
}

void Enemy::updateDie()
{
	//float xMove = 
}

void Enemy::changeToDieState()
{
	if (!isDying)
	{
		resetStates();
		isDying = true;
	}
}

void Enemy::displayJumping()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	if (isWaitingToAttack)
	{
		glBindTexture(GL_TEXTURE_2D, waitingToAttackTexture);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, jumpTextures[currentSprite]);
	}
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
	App::displayBoundingBox(diveXOffset, diveYOffset, diveXOffset + diveColliderWidth, diveYOffset + diveColliderHeight);
	App::displayBoundingBox(chaseXOffset, chaseYOffset, chaseXOffset + chaseColliderWidth, chaseYOffset + chaseColliderHeight);
	App::displayBoundingBox(jumpDownXOffset, jumpDownYOffset, jumpDownXOffset + jumpDownColliderWidth, jumpDownYOffset + jumpDownColliderHeight);
	App::displayBoundingBox(jumpUpXOffset, jumpUpYOffset, jumpUpXOffset + jumpUpColliderWidth, jumpUpYOffset + jumpUpColliderHeight);
	glPopMatrix();
}

void Enemy::displayRunning()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, runTextures[currentSprite]);
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0 + xReflectFactor, 1); glVertex2f(0, runningHeight);
	glTexCoord2f(1 - xReflectFactor, 1); glVertex2f(runningWidth, runningHeight);
	glTexCoord2f(1 - xReflectFactor, 0); glVertex2f(runningWidth, 0);
	glTexCoord2f(0 + xReflectFactor, 0); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	App::displayBoundingBox(colliderX, colliderY, colliderX + colliderWidth, colliderY + colliderHeight);
	App::displayBoundingBox(diveXOffset, diveYOffset, diveXOffset + diveColliderWidth, diveYOffset + diveColliderHeight);
	App::displayBoundingBox(chaseXOffset, chaseYOffset, chaseXOffset + chaseColliderWidth, chaseYOffset + chaseColliderHeight);
	App::displayBoundingBox(jumpDownXOffset, jumpDownYOffset, jumpDownXOffset + jumpDownColliderWidth, jumpDownYOffset + jumpDownColliderHeight);
	App::displayBoundingBox(jumpUpXOffset, jumpUpYOffset, jumpUpXOffset + jumpUpColliderWidth, jumpUpYOffset + jumpUpColliderHeight);
	glPopMatrix();
}

void Enemy::displayDiving()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	if (isWaitingToAttack)
	{
		glBindTexture(GL_TEXTURE_2D, waitingToAttackTexture);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, diveTextures[currentSprite]);
	}
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0 + xReflectFactor, 1); glVertex2f(0, divingHeight);
	glTexCoord2f(1 - xReflectFactor, 1); glVertex2f(divingWidth, divingHeight);
	glTexCoord2f(1 - xReflectFactor, 0); glVertex2f(divingWidth, 0);
	glTexCoord2f(0 + xReflectFactor, 0); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	App::displayBoundingBox(colliderX, colliderY, colliderX + colliderWidth, colliderY + colliderHeight);
	App::displayBoundingBox(diveXOffset, diveYOffset, diveXOffset + diveColliderWidth, diveYOffset + diveColliderHeight);
	App::displayBoundingBox(chaseXOffset, chaseYOffset, chaseXOffset + chaseColliderWidth, chaseYOffset + chaseColliderHeight);
	App::displayBoundingBox(jumpDownXOffset, jumpDownYOffset, jumpDownXOffset + jumpDownColliderWidth, jumpDownYOffset + jumpDownColliderHeight);
	App::displayBoundingBox(jumpUpXOffset, jumpUpYOffset, jumpUpXOffset + jumpUpColliderWidth, jumpUpYOffset + jumpUpColliderHeight);
	glPopMatrix();
}

void Enemy::display()
{
	if (isWalking)
	{
		displayWalking();
	}
	else if (isRunning)
	{
		displayRunning();
	}
	else if (isJumpingUp || isJumpingDown)
	{
		displayJumping();
	}
	else if (isDiving)
	{
		displayDiving();
	}
	else
	{
		displayIdle();
	}
}