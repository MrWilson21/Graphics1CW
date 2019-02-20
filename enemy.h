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
	void airMovementUpdate();
	void getCollisionUpdates();
	void calculateCollider(float blockX, float blockY, float blockWidth, float blockHeight, float xMove, float yMove);
	void detectPlayer();

	void calculateColliderBox();

	void changeToWalkingState();
	void changeToIdleState();
	void changeToRunningState();
	void changeToJumpingState();
	void changeToDivingState();
	void resetStates();

	void turnLeft();
	void turnRight();

	void displayWalking();
	void displayIdle();
	void displayJumping();
	void displayRunning();
	void displayDiving();

	void incrementSpriteCounter();

	float maxVelocityX;
	float maxVeloctyY;
	float walkingAcceleration;
	float deccelerationFactor;
	float maxRunningVelocityX;
	float runningAcceleration;
	float gravity;
	float airDeccelerationFactor;

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
	bool aimingLeft;

	bool isIdle;
	bool isWalking;
	bool isRunning;
	bool isJumping;
	bool isInAir;
	bool isDiving;
	bool isTouchingGround;
	bool isWalkingOfRightEdge;
	bool isWalkingOfLeftEdge;

	int currentSprite;
	int maxSprites;
	GLuint walkingTextures[12];
	GLuint idleTextures[2];
	GLuint diveTextures[4];
	GLuint runTextures[12];
	GLuint jumpTextures[6];

	float timeSinceFrameChange;
	float timeToWaitForNextWalkingFrame;
	float timeToWaitForNextIdleFrame;
	float timeToWaitForNextJumpingFrame;
	float timeToWaitForNextRunningFrame;
	float timeToWaitForNextDivingFrame;

	float timeSinceNotTouchingGround;
	float timeUntilChangeToJump;
	bool jumpLanding;

	//Display sizes for different player models, sizes based off of sprite image size
	float scaleFactor;
	float walkingWidth;
	float walkingHeight;
	float idleWidth;
	float idleHeight;
	float jumpingWidth;
	float jumpingHeight;
	float divingWidth;
	float divingHeight;
	float runningWidth;
	float runningHeight;

	World* parent;
};
