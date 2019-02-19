#include "enemy.h"
#include "world.h"

Enemy::Enemy(float startX, float startY, World* p)
{
	velocityX = 0.0;
	velocityY = 0.0;
	x = startX;
	y = startY;

	maxVelocityX = 30.0;
	maxVeloctyY = 1700.0;
	walkingAcceleration = 150.0;
	deccelerationFactor = 200.0;
	gravity = 350;
	airDeccelerationFactor = 150.0;

	xReflectFactor = 0;
	facingLeft = false;

	currentSprite = 0;
	maxSprites = 0;
	walkingTextures[12];
	idleTextures[2];

	timeSinceFrameChange = 0.0;
	timeToWaitForNextWalkingFrame = 1.5;
	timeToWaitForNextIdleFrame = 0.8;

	timeSinceNotTouchingGround = 0.0;
	timeUntilChangeToJump = 0.01;

	//Display sizes for different player models, sizes based off of sprite image size
	scaleFactor = 0.25;
	walkingHeight = 39.0 * scaleFactor;
	walkingWidth = 36.0 * scaleFactor;
	idleHeight = 37.0 * scaleFactor;
	idleWidth = 38.0 * scaleFactor;

	parent = p;

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
	chaseColliderHeight = 50;
	diveColliderWidth = 45;
	diveColliderHeight = 12;
	jumpUpColliderWidth = 35;
	jumpUpColliderHeight = 30;
	jumpDownColliderWidth = 35;
	jumpDownColliderHeight = 30;

	chaseXOffset = -15;
	chaseYOffset = 5;
	diveXOffset = 20;
	diveYOffset = 0;
	jumpUpXOffset = 20;
	jumpUpYOffset = 25;
	jumpDownXOffset = 20;
	jumpDownYOffset = -25;

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
}

void Enemy::detectPlayer()
{

}

void Enemy::update()
{
	detectPlayer();

	//Get player input and change players position and orientation
	getMovementUpdates();

	//GetShape of collider box for this frame
	calculateColliderBox();

	//After player is moved calculate collisions and make adjustments before rendering frame
	getCollisionUpdates();

	if (isWalkingOfLeftEdge)
	{
		turnRight();
	}
	else if (isWalkingOfRightEdge)
	{
		turnLeft();
	}
}

void Enemy::turnLeft()
{
	velocityX = 0.0;
	facingLeft = true;
}

void Enemy::turnRight()
{
	velocityX = 0.0;
	facingLeft = false;
}

//Get player input and move player based on current state and inputs given
void Enemy::getMovementUpdates()
{
	if (isWalking)
	{
		groundMovementUpdate();
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
}

void Enemy::groundMovementUpdate()
{
	if (facingLeft)
	{
		velocityX -= walkingAcceleration * App::deltaTime;
		if (velocityX < -maxVelocityX)
		{
			velocityX = -maxVelocityX;
		}
	}
	else
	{
		velocityX += walkingAcceleration * App::deltaTime;
		if (velocityX > maxVelocityX)
		{
			velocityX = maxVelocityX;
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

	incrementSpriteCounter();
}

void Enemy::calculateCollider(float blockX, float blockY, float blockWidth, float blockHeight, float xMove, float yMove)
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
			turnRight();
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
			turnLeft();
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
			if (x >= blockX)
			{
				isWalkingOfLeftEdge = false;
			}
			if (x + colliderWidth <= blockX + blockWidth)
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

void Enemy::display()
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