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
	deccelerationFactor = 5.0;
	gravity = 350;
	airDeccelerationFactor = 0.0;

	jumpUpHeight = 190.0;
	jumpUpXVelocity = 120.0;
	jumpDownHeight = 75.0;
	jumpDownXVelocity = 120.0;
	diveHeight = 65.0;
	diveXVelocity = 240;

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
	timeUntilChangeToJump = 0.1;

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
	dyingWidth = 40 * scaleFactor;
	dyingHeight = 42 * scaleFactor;

	delayBeforeAttacking = 0.7;
	delayBetweenDives = 3.5;
	delayBetweenJumps = 1.2;
	timeSinceStartingAttack = 0.0;
	timeSinceAttacking = 0.0;
	delayAfterLandingAttack = 0.6;
	timeSinceLandingAttack = 0.0;

	dieFlySpeed = 200.0;
	timeUntilFadeOut = 0.2;
	dieDecceleration = 300.0;
	fadeOutSpeed = 2.0;
	timeSinceDying = 0.0;
	transparency = 1.0;

	isOnRotatingCollider = false;
	timeSinceLeavingRotatingCollider = 0.0;

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
	dyingTexture = App::loadPNG("knuckles/dying.png");
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
		if (velocityX - airDeccelerationFactor * App::deltaTime * abs(velocityX) > 0.0)
		{
			velocityX -= airDeccelerationFactor * App::deltaTime * abs(velocityX);
		}
		else
		{
			velocityX = 0.0;
		}
	}
	else
	{
		if (velocityX + airDeccelerationFactor * App::deltaTime * abs(velocityX) < 0.0)
		{
			velocityX += airDeccelerationFactor * App::deltaTime * abs(velocityX);
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
			velocityX += deccelerationFactor * App::deltaTime * abs(velocityX);
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
			velocityX -= deccelerationFactor * App::deltaTime * abs(velocityX);
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

void Enemy::calculateRotatingCollider(App::Point b0, App::Point b1, App::Point b2, App::Point b3, float rotation, float blockFriction)
{
	//Test if colliding
	//Get closest edge from both objects
	//Find axis with least travel distance to seperate
	//Move player by minimum travel distance in direction of minimum axis

	if (rotation / 90 == 1.0 || rotation == 0)
	{
		calculateCollider(b3.x, b3.y, b1.x - b3.x, b1.y - b3.y, 0.0, 0.0);
		return;
	}

	//b0 and p0 are top left corners going clockwise
	App::Point a0 = App::Point{ x + colliderX, y + colliderY + colliderHeight };
	App::Point a1 = App::Point{ x + colliderX + colliderWidth, y + colliderY + colliderHeight };
	App::Point a2 = App::Point{ x + colliderX + colliderWidth, y + colliderY };
	App::Point a3 = App::Point{ x + colliderX, y + colliderY };

	App::Point axis1 = (b2 - b3) / (b2 - b3).abs();
	App::Point axis2 = (b1 - b2) / (b1 - b2).abs();
	App::Point axis3 = (a2 - a3) / (a2 - a3).abs();
	App::Point axis4 = (a1 - a2) / (a1 - a2).abs();

	//Test axis 1
	float projections[4];
	projections[0] = (a0 - b3).dot(axis1);
	projections[1] = (a1 - b3).dot(axis1);
	projections[2] = (a2 - b3).dot(axis1);
	projections[3] = (a3 - b3).dot(axis1);

	std::sort(projections, projections + 4);

	if (projections[0] > (b2 - b3).abs() || projections[3] < 0)
	{
		return;
	}

	//Calculate distances from each edge
	float distToRightBlock = (b2 - b3).abs() - projections[0];
	float distToLeftBlock = projections[3];

	//Test axis 2
	projections[0] = (a0 - b2).dot(axis2);
	projections[1] = (a1 - b2).dot(axis2);
	projections[2] = (a2 - b2).dot(axis2);
	projections[3] = (a3 - b2).dot(axis2);

	std::sort(projections, projections + 4);

	if (projections[0] > (b1 - b2).abs() || projections[3] < 0)
	{
		return;
	}

	//Calculate distances from each edge
	float distToTopBlock = (b1 - b2).abs() - projections[0];
	float distToBottomBlock = projections[3];

	//Test axis 3
	projections[0] = (b0 - a3).dot(axis3);
	projections[1] = (b1 - a3).dot(axis3);
	projections[2] = (b2 - a3).dot(axis3);
	projections[3] = (b3 - a3).dot(axis3);

	std::sort(projections, projections + 4);

	if (projections[0] > (a2 - a3).abs() || projections[3] < 0)
	{
		return;
	}

	//Calculate distances from each edge
	float distToRightPlayer = (a2 - a3).abs() - projections[0];
	float distToLeftPlayer = projections[3];

	//Test axis 4
	projections[0] = (b0 - a2).dot(axis4);
	projections[1] = (b1 - a2).dot(axis4);
	projections[2] = (b2 - a2).dot(axis4);
	projections[3] = (b3 - a2).dot(axis4);

	std::sort(projections, projections + 4);

	if (projections[0] > (a1 - a2).abs() || projections[3] < 0)
	{
		return;
	}

	//Calculate distances from each edge
	float distToTopPlayer = (a1 - a2).abs() - projections[0];
	float distToBottomPlayer = projections[3];

	App::Point playerAxis;
	App::Point blockAxis;
	App::Point blockSlope;

	bool playerEdgeIntersect = false;
	bool verticalIntersect = false;

	//Get the two corners that surround the point where block intersected
	if (distToRightBlock < distToLeftBlock &&
		distToRightBlock < distToTopBlock &&
		distToRightBlock < distToBottomBlock)
	{
		blockAxis = axis1 * distToRightBlock;
		blockSlope = (b1 - b2);
	}
	else if (distToLeftBlock < distToRightBlock &&
		distToLeftBlock < distToTopBlock &&
		distToLeftBlock < distToBottomBlock)
	{
		blockAxis = axis1 * -distToLeftBlock;
		blockSlope = (b0 - b3);
	}
	else if (distToBottomBlock < distToLeftBlock &&
		distToBottomBlock < distToTopBlock &&
		distToBottomBlock < distToRightBlock)
	{
		blockAxis = axis2 * -distToBottomBlock;
		blockSlope = (b3 - b2);
	}
	else
	{
		blockAxis = axis2 * distToTopBlock;
		blockSlope = (b0 - b1);
	}

	//Get the two corners that surround the point where player intersected 
	if (distToRightPlayer < distToLeftPlayer &&
		distToRightPlayer < distToTopPlayer &&
		distToRightPlayer < distToBottomPlayer)
	{
		playerAxis = axis3 * distToRightPlayer;
		verticalIntersect = true;
	}
	else if (distToLeftPlayer < distToRightPlayer &&
		distToLeftPlayer < distToTopPlayer &&
		distToLeftPlayer < distToBottomPlayer)
	{
		playerAxis = axis3 * -distToLeftPlayer;
		verticalIntersect = true;
	}
	else if (distToBottomPlayer < distToLeftPlayer &&
		distToBottomPlayer < distToTopPlayer &&
		distToBottomPlayer < distToRightPlayer)
	{
		playerAxis = axis4 * -distToBottomPlayer;
		isTouchingGround = true;
		timeSinceNotTouchingGround = 0.0;
	}
	else
	{
		playerAxis = axis4 * distToTopPlayer;
	}

	//Find minimum translation vector (mtv)
	App::Point mtVector;
	App::Point currentVelocity = App::Point{ velocityX * (float)App::deltaTime, velocityY * (float)App::deltaTime };

	if (blockAxis.abs() > playerAxis.abs())
	{
		mtVector = playerAxis * -1;
		playerEdgeIntersect = true;
	}
	else
	{
		mtVector = blockAxis;
	}

	//If either velocity or mtv somehow have 0 magnitude then escape response 
	if (mtVector.abs() <= 0)
	{
		return;
	}
	if (currentVelocity.abs() <= 0)
	{
		return;
	}

	//Get direction of player movement by reversing player velocity
	App::Point velDirection = currentVelocity / -currentVelocity.abs();
	//Get mtv direction vector
	App::Point mtvDirection = mtVector / mtVector.abs();

	//Move in opposite direction of current velocity with magnitude of mtv / cos(angle)
	float moveAngle = velDirection.dot(mtvDirection) / (velDirection.abs() * mtvDirection.abs());
	float fractionOfVelocity = mtVector.abs() / currentVelocity.abs();

	//Get vector to move player by
	//App::Point moveVector = velDirection * (currentVelocity.abs() * fractionOfVelocity);
	App::Point moveVector = mtVector;
	App::Point newVelocity;
	App::Point currentDirection = currentVelocity / currentVelocity.abs();

	if (playerEdgeIntersect)
	{
		if (verticalIntersect)
		{
			velocityX = 0;
		}
		else
		{
			velocityY = 0;
		}
	}
	else
	{
		float velocityAngle = currentVelocity.dot(blockSlope) / (currentVelocity.abs() * blockSlope.abs());
		velocityAngle = acos(velocityAngle) * App::radToDeg;
		if (velocityAngle > 90)
		{
			newVelocity = blockSlope * -blockSlope.abs();
		}
		else
		{
			newVelocity = blockSlope * blockSlope.abs();
		}
		if (currentVelocity.abs() < 1 && currentVelocity.x * newVelocity.x < 0)
		{
			newVelocity = newVelocity * -1;
		}


		if (currentVelocity.abs() > 0 && mtVector.abs() > 0)
		{
			//currentVelocity.y += gravity * App::deltaTime;
			float angle = mtVector.dot(currentVelocity) / (mtVector.abs() * currentVelocity.abs());
			//angle = abs(acos(angle) * App::radToDeg / 90);
			angle = abs(angle);

			//Get x component of new velocity magnitude
			App::Point newVelocityMagnitude = App::Point{ 0, 0 };
			if (abs(velocityX) > 0)
			{
				if (isOnRotatingCollider || timeSinceLeavingRotatingCollider < timeUntilChangeToJump)
				{
					newVelocityMagnitude.x = abs(velocityX);
				}
				else
				{
					newVelocityMagnitude.x = abs(velocityX) - abs(velocityX) * (angle);
				}
			}

			//Get y component of new velocity magnitude
			if (abs(velocityY) > 0)
			{
				if (isOnRotatingCollider || timeSinceLeavingRotatingCollider < timeUntilChangeToJump)
				{
					newVelocityMagnitude.y = abs(velocityY);
				}
				else
				{
					newVelocityMagnitude.y = abs(velocityY) - abs(velocityY) * (angle);
				}
				newVelocityMagnitude.y = abs(velocityY) - abs(velocityY) * (angle);
			}

			//Combine angle and magnitude of new velocity
			newVelocity = newVelocity / newVelocity.abs();
			newVelocity = newVelocity * newVelocityMagnitude.abs();
			velocityX = newVelocity.x;
			velocityY = newVelocity.y;
		}
	}

	x += moveVector.x;
	y += moveVector.y;

	x += velocityX * fractionOfVelocity * App::deltaTime;
	y += velocityY * fractionOfVelocity * App::deltaTime;

	if (newVelocity.abs() < 0.4)
	{
		velocityX = 0.0;
		velocityY = 0.0;
	}

	isOnRotatingCollider = true;
	timeSinceLeavingRotatingCollider = 0.0;
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

	isOnRotatingCollider = false;
	for (RotatingBlock block : parent->rotatingBlocks)
	{
		calculateRotatingCollider(block.p0, block.p1, block.p2, block.p3, block.rotation, block.friction);
	}
	if (!isOnRotatingCollider)
	{
		timeSinceLeavingRotatingCollider += App::deltaTime;
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

void Enemy::die(float angle, bool facingLeft)
{
	changeToDieState();
	dieAngle = angle;
	this->facingLeft = facingLeft;
}

bool Enemy::operator==(const Enemy & e)
{
	if (ID == e.ID)
	{
		return true;
	}
	return false;
}

void Enemy::updateDie()
{
	x += cos(dieAngle*App::degToRad) * dieFlySpeed * App::deltaTime;
	y += sin(dieAngle*App::degToRad) * dieFlySpeed * App::deltaTime;

	timeSinceDying += App::deltaTime;
	dieFlySpeed -= dieDecceleration * App::deltaTime;
	if (timeSinceDying > timeUntilFadeOut)
	{
		transparency -= fadeOutSpeed * App::deltaTime;
		if (transparency < 0.0)
		{
			parent->enemies.erase(std::find(parent->enemies.begin(), parent->enemies.end(), *this));
		}
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

void Enemy::displayDying()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, dyingTexture);
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glRotatef(dieAngle - 90, 0.0, 0.0, 1.0);
	glColor4f(1.0, 1.0, 1.0, transparency);
	glBegin(GL_POLYGON);
	glTexCoord2f(0 + xReflectFactor, 1); glVertex2f(0, dyingHeight);
	glTexCoord2f(1 - xReflectFactor, 1); glVertex2f(dyingWidth, dyingHeight);
	glTexCoord2f(1 - xReflectFactor, 0); glVertex2f(dyingWidth, 0);
	glTexCoord2f(0 + xReflectFactor, 0); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Enemy::display()
{
	if (isDying)
	{
		displayDying();
	}
	else if (isWalking)
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