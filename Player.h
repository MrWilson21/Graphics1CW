#pragma once
#include "app.h"
#include "staticBlock.h"
#include "movingblock.h"

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
		void update();
		void display();
		void displayHearts(int heartNo, int spriteToRender, float screenWidth, float screenHeight);
		void loadSprites();

		void createPartical(float x1, float y1, float width, float height, float scale, string sprite);
		void addScore(int score);

		int gemsCollected;
		int health;

	private:
		void getMovementUpdates();
		void groundMovementUpdate();
		void airMovementUpdate();
		void groundMove();
		void airMove();
		void getCollisionUpdates();
		void calculateCollider(float blockX, float blockY, float blockWidth, float blockHeight, float xMove, float yMove);
		void calculateRotatingCollider(App::Point b0, App::Point b1, App::Point b2, App::Point b3, float rotation);

		void calculateColliderBox();

		void changeToWalkingState();
		void changeToIdleState();
		void changeToJumpingState();
		void changeToDyingState();
		void resetStates();

		void displayWalking();
		void displayJumping();
		void displayIdle();
		void displayDying();
		void displayAttacking();
		void incrementSpriteCounter();

		void displayFist();
		void changeToAttackingState();
		void updateFist();
		void collideFistWithEnemies();

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
		bool isWalking;
		bool isJumping;
		bool isTouchingGround;
		bool isAttacking;
		bool isDying;

		int currentSprite;
		int maxSprites;
		GLuint walkingTextures[12];
		GLuint idleTextures[18];
		GLuint jumpingTextures[9];
		GLuint attackPowerTextures[10];
		GLuint attackNoPowerTextures[10];
		GLuint heartTextures[10];
		GLuint dyingTextures[4];

		float timeSinceFrameChange;
		float timeSinceIdleAnimation;
		float timeToWaitForNextWalkingFrame;
		float timeToWaitUntilIdleAnimation;
		float timeToWaitForNextIdleFrame;
		float timeToWaitForNextJumpingFrame;
		float timeToWaitForNextAttackingFrame;
		float timeToWaitForNextDyingFrame;
		
		bool isInvinsible;
		float timeBetweenHits;
		float timeSinceLastHit;
		float timeBetweenFlashes;
		float timeSinceFlash;
		
		float timeSinceLeavingRotatingCollider;
		bool isOnRotatingCollider;

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
		float attackingHeight;
		float attackingWidth;
		float dyingWidth;
		float dyingHeight;

		World* parent;

		float fistX;
		float fistY;
		float fistWidth;
		float fistHeight;
		float fistColliderX;
		float fistColliderY;
		float fistColliderWidth;
		float fistColliderHeight;
		GLuint fistTexture;
		float fistTransparency;
		float fistVelocityX;
		bool fistColliderActive;
		bool fistActive;
		bool fistFadingIn;
		bool fistFadingOut;
		bool fistFacingLeft;
		float fistDistanceToMove;
		float fistDistanceBeforeFadingOut;
		float fistDistanceMoved;
		float fistScale;
		float fistXOffset;
		float fistFadeInSpeed;
		float fistFadeOutSpeed;
		int gemsNeededToAttack;

		float timeSinceAttack;
		float delayBetweenAttacks;

		float heartOffsetFromTop;
		float heartOffsetFromRight;
		float heartSpacing;
		float heartWidth;
		float heartHeight;
		float heartScale;
};