#pragma once
#include "app.h"
#include "staticBlock.h"

class Player
{
	public:
		float velocityX;
		float velocityY;
		float x;
		float y;
		float width;
		float height;

		Player(float startX, float startY, float scale);
		void updatePlayer(std::vector<StaticBlock> staticBlocks);
		void displayPlayer();
		void loadSprites();

	private:
		void getMovementUpdates();
		void getCollisionUpdates(std::vector<StaticBlock> staticBlocks);
		void sideWaysBlockCollisions(std::vector<StaticBlock> staticBlocks);

		void changeToWalkingState();
		void changeToRunningState();
		void changeToIdleState();
		void changeToJumpingState();
		void resetStates();

		void displayWalking();
		void displayIdle();
		void incrementSpriteCounter();

		float maxSpeed;
		float walkingAcceleration;
		float deccelerationFactor;
		
		int xReflectFactor;
		bool facingLeft;

		bool isIdle;
		bool isRunning;
		bool isWalking;
		bool isJumping;

		int currentSprite;
		int maxSprites;
		GLuint walkingTextures[12];
		GLuint idleTextures[18];

		float timeSinceFrameChange;
		float timeSinceIdleAnimation;
		float timeToWaitUntilIdleAnimation;
		float timeToWaitForNextIdleFrame;
		float timeToWaitForNextWalkingFrame;

		float scaleFactor;
		float walkingHeight;
		float walkingWidth;
		float idleHeight;
		float idleWidth;
};