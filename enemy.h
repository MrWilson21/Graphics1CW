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

	int ID;

	bool isDying;

	Enemy(float startX, float startY, World* p, int ID);
	void update();
	void display();
	void loadSprites();
	void die(float angle, bool facingLeft);

	bool operator==(const Enemy& e);

private:
	void getMovementUpdates();
	void groundMovementUpdate();
	void airMovementUpdate();
	void getCollisionUpdates();
	void calculateCollider(float blockX, float blockY, float blockWidth, float blockHeight, float xMove, float yMove);
	void calculateRotatingCollider(App::Point b0, App::Point b1, App::Point b2, App::Point b3, float rotation);
	void detectPlayer();
	void attack();
	void changeToDieState();
	void updateDie();

	void calculateColliderBox();

	void changeToWalkingState();
	void changeToIdleState();
	void changeToRunningState();
	void changeToJumpingState();
	void changeToDivingState();
	void changeToWaitingToAttack();
	void resetStates();

	void turnLeft();
	void turnRight();

	void displayWalking();
	void displayIdle();
	void displayJumping();
	void displayRunning();
	void displayDiving();
	void displayDying();

	void incrementSpriteCounter();

	float maxVelocityX;
	float maxVeloctyY;
	float walkingAcceleration;
	float deccelerationFactor;
	float maxRunningVelocityX;
	float runningAcceleration;
	float gravity;
	float airDeccelerationFactor;

	float jumpUpHeight;
	float jumpUpXVelocity;
	float jumpDownHeight;
	float jumpDownXVelocity;
	float diveHeight;
	float diveXVelocity;

	float delayBeforeAttacking;
	float delayBetweenJumps;
	float delayBetweenDives;
	float timeSinceStartingAttack;
	float timeSinceAttacking;
	float delayAfterLandingAttack;
	float timeSinceLandingAttack;

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
	bool isJumpingUp;
	bool isJumpingDown;
	bool isInAir;
	bool isTouchingLeft;
	bool isTouchingRight;
	bool isDiving;
	bool isTouchingGround;
	bool isWalkingOfRightEdge;
	bool isWalkingOfLeftEdge;
	bool isWaitingToAttack;
	bool isAttacking;

	int currentSprite;
	int maxSprites;
	GLuint walkingTextures[12];
	GLuint idleTextures[2];
	GLuint diveTextures[4];
	GLuint runTextures[12];
	GLuint jumpTextures[10];
	GLuint waitingToAttackTexture;
	GLuint dyingTexture;

	float timeSinceFrameChange;
	float timeToWaitForNextWalkingFrame;
	float timeToWaitForNextIdleFrame;
	float timeToWaitForNextJumpingFrame;
	float timeToWaitForNextRunningFrame;
	float timeToWaitForNextDivingFrame;

	float timeSinceNotTouchingGround;
	float timeUntilChangeToJump;
	bool jumpLanding;

	//Display sizes for different enemy models, sizes based off of sprite image size
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
	float dyingHeight;
	float dyingWidth;

	bool isOnRotatingCollider;
	float timeSinceLeavingRotatingCollider;

	float dieFlySpeed;
	float dieAngle;
	float timeUntilFadeOut;
	float dieDecceleration;
	float fadeOutSpeed;
	float timeSinceDying;
	float transparency;

	World* parent;
};
