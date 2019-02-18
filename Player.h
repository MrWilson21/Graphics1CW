#pragma once
#include "app.h"
#include "staticBlock.h"

//Pre define class and include world.h in player.cpp to fix circular definition problem 
class World;
class Player
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

		Player(float startX, float startY, World* p);
		void updatePlayer(std::vector<StaticBlock> staticBlocks);
		void displayPlayer();
		void loadSprites();

	private:
		void getMovementUpdates();
		void groundMovementUpdate();
		void airMovementUpdate();
		void groundMove();
		void airMove();
		void getCollisionUpdates(std::vector<StaticBlock> staticBlocks);
		void Player::calculateCollider(float blockX, float blockY, float blockWidth, float blockHeight);

		void calculateColliderBox();

		void changeToWalkingState();
		void changeToRunningState();
		void changeToIdleState();
		void changeToJumpingState();
		void resetStates();

		void displayWalking();
		void displayJumping();
		void displayIdle();
		void incrementSpriteCounter();

		float maxVelocityX;
		float maxVeloctyY;
		float walkingAcceleration;
		float deccelerationFactor;
		float jumpSpeed;
		float gravity;
		float airAcceleration;
		float airDeccelerationFactor;
		float maxAirVelocityX;

		float xMoveThisFrame;
		float yMoveThisFrame;
		
		int xReflectFactor;
		bool facingLeft;

		bool isIdle;
		bool isRunning;
		bool isWalking;
		bool isJumping;
		bool isTouchingGround;

		int currentSprite;
		int maxSprites;
		GLuint walkingTextures[12];
		GLuint idleTextures[18];
		GLuint jumpingTextures[9];

		float timeSinceFrameChange;
		float timeSinceIdleAnimation;
		float timeToWaitForNextWalkingFrame;
		float timeToWaitUntilIdleAnimation;
		float timeToWaitForNextIdleFrame;
		float timeToWaitForNextJumpingFrame;

		float timeSinceNotTouchingGround;
		float timeUntilChangeToJump;
		bool jumpLanding;

		//Display sizes for different player models, sizes based off of sprite image size
		float scaleFactor;
		float walkingHeight;
		float walkingWidth;
		float idleHeight;
		float idleWidth;
		float jumpingHeight;
		float jumpingWidth;

		World* parent;
};