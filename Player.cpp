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
	walkingAcceleration = 400.0;
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
	timeToWaitForNextDyingFrame = 0.4;

	timeSinceNotTouchingGround = 0.0;
	timeUntilChangeToJump = 0.05;
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
	dyingWidth = 27 * scaleFactor;
	dyingHeight = 68 * scaleFactor;

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
	
	isInvinsible = false;
	timeBetweenHits = 1.5;
	timeSinceLastHit = 0.0;
	timeBetweenFlashes = 0.12;
	timeSinceFlash = 0.0;
	
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
		timeSinceFrameChange += App::deltaTime;
	}
	else if (isDying)
	{
		if (timeSinceFrameChange > timeToWaitForNextDyingFrame)
		{
			if (currentSprite < maxSprites - 1)
			{
				currentSprite += 1;
			}
			else
			{
				parent->signalGameEnd();
			}
			timeSinceFrameChange = 0.0;
		}
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

	for (int i = 0; i < 4; i++)
	{
		string imgSrc = "shaggy/death/" + to_string(i) + ".png";
		char *cstr = &imgSrc[0u];
		dyingTextures[i] = App::loadPNG(cstr);
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
	if (health < 1)
	{
		changeToDyingState();
	}

	//Change to attack state if attack input detected
	if (!isAttacking && !isDying)
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
	if (velocityX == 0 && !(isAttacking || isDying || isJumping || !isTouchingGround))
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
	if (isJumping || ((isAttacking || isDying) && !isTouchingGround))
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

	if (App::keys[VK_UP] && !(isAttacking || isDying))
	{
		velocityY = jumpSpeed;
		changeToJumpingState();
	}
	else if (abs(velocityX) > 0 && !(isAttacking || isDying))
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
	if (App::keys[VK_LEFT] && !(isAttacking || isDying))
	{
		velocityX -= walkingAcceleration * App::deltaTime;
		if (velocityX < -maxVelocityX)
		{
			velocityX = -maxVelocityX;
		}
	}
	else if (App::keys[VK_RIGHT] && !(isAttacking || isDying))
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
		if (App::keys[VK_LEFT] && !(isAttacking || isDying))
		{
			velocityX -= airAcceleration * App::deltaTime;
			if (velocityX < -maxAirVelocityX)
			{
				velocityX = -maxAirVelocityX;
			}
		}
		else if (App::keys[VK_RIGHT] && !(isAttacking || isDying))
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

void Player::calculateRotatingCollider(App::Point b0, App::Point b1, App::Point b2, App::Point b3, float rotation, float blockFriction)
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
	App::Point a0 = App::Point{ x + colliderX, y + colliderY + colliderHeight};
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
	App::Point currentVelocity = App::Point{ velocityX * (float)App::deltaTime, velocityY * (float)App::deltaTime};

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
		cout << "mtv";
		return;
	}
	if (currentVelocity.abs() <= 0)
	{
		cout << "currentVelocity";
		return;
	}

	//Get direction of player movement by reversing player velocity
	App::Point velDirection = currentVelocity / -currentVelocity.abs();
	//Get mtv direction vector
	App::Point mtvDirection = mtVector / mtVector.abs();

	//Move in opposite direction of current velocity with magnitude of mtv / cos(angle)
	float moveAngle = velDirection.dot(mtvDirection) / (velDirection.abs() * mtvDirection.abs());
	//cout << moveAngle <<  "      " << acos(moveAngle) * App::radToDeg <<  "\n";
	//float fractionOfVelocity = mtVector.abs() / moveAngle;
	float fractionOfVelocity = mtVector.abs() / currentVelocity.abs();

	//Get vector to move player by
	App::Point moveVector = velDirection * (currentVelocity.abs() * fractionOfVelocity);
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

		if (currentVelocity.abs() > 0 && mtVector.abs() > 0)
		{
			//Get x component of new velocity magnitude
			App::Point newVelocityMagnitude = App::Point{ 0, 0 };
			if (abs(velocityX) > 0)
			{
				float angle = mtVector.dot(App::Point{ -velocityX,0 }) / (mtVector.abs() * App::Point{ -velocityX,0 }.abs());
				if (angle < 0)
				{
					angle = 0;
				}
				newVelocityMagnitude.x = velocityX - velocityX * angle;
			}

			//Get y component of new velocity magnitude
			if (abs(velocityY) > 0)
			{
				float angle = mtVector.dot(App::Point{ 0, -velocityY }) / (mtVector.abs() * App::Point{ 0,-velocityY }.abs());
				if (angle < 0)
				{
					angle = 0;
				}
				newVelocityMagnitude.y = velocityY - velocityY * angle;
			}

			//Combine angle and magnitude of new velocity
			//float angle = mtVector.dot(currentVelocity) / (mtVector.abs() * currentVelocity.abs());
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

	glColor3f(1.0, 0.0, 0.0);
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

	for (RotatingBlock block : parent->rotatingBlocks)
	{
		calculateRotatingCollider(block.p0, block.p1, block.p2, block.p3, block.rotation, block.friction);
	}

	calculateCollider(parent->rightEdge, 0, 100, parent->worldSizeY, 0.0, 0.0);
	calculateCollider(-100 + parent->leftEdge, 0, 100, parent->worldSizeY, 0.0, 0.0);
	calculateCollider(0, parent->topEdge, parent->worldSizeX, 100, 0.0, 0.0);
	calculateCollider(0, -100 + parent->bottomEdge, parent->worldSizeX, 100, 0.0, 0.0);

	if (!isTouchingGround && !isJumping && !isAttacking && !isDying)
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

	if (!isInvinsible && !isDying)
	{
		for (Enemy enemy : parent->enemies)
		{
			if (x + colliderX < enemy.x + enemy.colliderX + enemy.colliderWidth &&
				x + colliderX + colliderWidth > enemy.x + enemy.colliderX &&
				y + colliderY < enemy.y + enemy.colliderY + enemy.colliderHeight &&
				y + colliderY + colliderHeight > enemy.y + enemy.colliderY)
			{
				health -= 1;
				isInvinsible = true;
				timeSinceLastHit = 0.0;
				break;
			}
		}
	}
	else
	{
		timeSinceLastHit += App::deltaTime;
		if (timeSinceLastHit > timeBetweenHits)
		{
			isInvinsible = false;
		}
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
void Player::changeToDyingState()
{
	if (!isDying)
	{
		resetStates();
		maxSprites = 4;
		isDying = true;
		isInvinsible = false;
	}
}
void Player::resetStates()
{
	currentSprite = 0;
	timeSinceFrameChange = 0.0;
	isJumping = false;
	isWalking = false;
	isIdle = false;
	isAttacking = false;
	isDying = false;
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

void Player::displayDying()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, dyingTextures[currentSprite]);
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0 + xReflectFactor, 1); glVertex2f(0, dyingHeight);
	glTexCoord2f(1 - xReflectFactor, 1); glVertex2f(dyingWidth, dyingHeight);
	glTexCoord2f(1 - xReflectFactor, 0); glVertex2f(dyingWidth, 0);
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
	if (!isInvinsible || timeSinceFlash < timeBetweenFlashes)
	{
		if (isWalking)
		{
			displayWalking();
		}
		else if (isJumping)
		{
			displayJumping();
		}
		else if (isAttacking)
		{
			displayAttacking();
		}
		else if (isDying)
		{
			displayDying();
		}
		else
		{
			displayIdle();
		}
	}

	timeSinceFlash += App::deltaTime;
	if (timeSinceFlash > timeBetweenFlashes * 2)
	{
		timeSinceFlash = 0.0;
	}
	
	if (fistActive)
	{
		displayFist();
	}
}