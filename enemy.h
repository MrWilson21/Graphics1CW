#pragma once
#include "app.h"
#include "staticBlock.h"
#include "movingblock.h"

//Pre define class and include world.h in player.cpp to fix circular definition problem 
class World;
class Enemy
{
public:
	float velocityX;
	float velocityY;
	float x;
	float y;

	float colliderX;
	float colliderY;
	float colliderWidth;
	float colliderHeight;

	Enemy(float startX, float startY, World* p);
	void update();
	void display();
	void loadSprites();

private:
	void getMovementUpdates();
	void groundMovementUpdate();
	void getCollisionUpdates();
	void calculateCollider(float blockX, float blockY, float blockWidth, float blockHeight, float xMove, float yMove);
	void detectPlayer();

	void calculateColliderBox();

	void changeToWalkingState();
	void changeToIdleState();
	void resetStates();

	void turnLeft();
	void turnRight();

	void displayWalking();
	void displayIdle();
	void incrementSpriteCounter();

	float maxVelocityX;
	float maxVeloctyY;
	float walkingAcceleration;
	float deccelerationFactor;
	float gravity;
	float airAcceleration;
	float airDeccelerationFactor;
	float maxAirVelocityX;

	float chaseXOffset;
	float chaseYOffset;
	float chaseColliderWidth;
	float chaseColliderHeight;
	float diveXOffset;
	float diveYOffset;
	float diveColliderWidth;
	float diveColliderHeight;
	float jumpUpXOffset;
	float jumpUpYOffset;
	float jumpUpColliderWidth;
	float jumpUpColliderHeight;
	float jumpDownXOffset;
	float jumpDownYOffset;
	float jumpDownColliderWidth;
	float jumpDownColliderHeight;

	float xMoveThisFrame;
	float yMoveThisFrame;

	int xReflectFactor;
	bool facingLeft;

	bool isIdle;
	bool isWalking;
	bool isTouchingGround;
	bool isWalkingOfRightEdge;
	bool isWalkingOfLeftEdge;

	int currentSprite;
	int maxSprites;
	GLuint walkingTextures[12];
	GLuint idleTextures[2];

	float timeSinceFrameChange;
	float timeToWaitForNextWalkingFrame;
	float timeToWaitForNextIdleFrame;

	float timeSinceNotTouchingGround;
	float timeUntilChangeToJump;
	bool jumpLanding;

	//Display sizes for different player models, sizes based off of sprite image size
	float scaleFactor;
	float walkingHeight;
	float walkingWidth;
	float idleHeight;
	float idleWidth;

	World* parent;
};
