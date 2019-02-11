#pragma once
#include "app.h"

class Player
{
	public:
		float speed;
		float x;
		float y;

		Player();
		void updatePlayer();
		void displayPlayer();
		void loadSprites();

	private:
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