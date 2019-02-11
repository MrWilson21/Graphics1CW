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
		float maxSpeed;
		float walkingAcceleration;
		bool facingLeft;
		bool isMoving;

		int currentSprite;
		GLuint walkingTextures[12];
		GLuint playerTexture;

		float timeSinceFrameChange;
		float timeBetweenFrameChanges;
};