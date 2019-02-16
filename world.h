#pragma once
#include "app.h"
#include "staticBlock.h"
#include "player.h"

class World
{
public:
	World();
	void init();
	void update();
	void display();
	void moveCamera(float screenWidth, float screenHeight);
	void displayBackground(float screenWidth, float screenHeight);
	void displayPlayerCameraBox(float screenWidth, float screenHeight);

	Player player = Player(250, 250, 0.2);
	std::vector<StaticBlock> staticBlocks;

private:

	float worldStartX = 0.0;
	float worldStartY = 0.0;
	float worldSizeX = 500.0;
	float worldSizeY = 500.0;

	float cameraX;
	float cameraY;
	float playerCameraBoxRadiusX = 30;
	float playerCameraBoxRadiusY = 40;

	//Textures for parallax background
	GLuint staticBackground;
	GLuint backGroundFront;
	GLuint backGroundMiddle;
	GLuint backGroundBack;

	//Scrollng speed of each background piece
	float backGroundFrontSpeedX = 0.001;
	float backGroundMiddleSpeedX;
	float backGroundBackSpeedX;

	float backGroundFrontSpeedY = 0.0005;
	float backGroundMiddleSpeedY;
	float backGroundBackSpeedY;

	//Aspect ratio of each background
	float backGroundStaticWidth = 272.0 / 104.0;
	float backGroundFrontWidth;
	float backGroundMiddleWidth;
	float backGroundBackWidth;

	//Scale of each backGround;
	float backGroundFrontScale = 0.5;
	float backGroundMiddleScale;
	float backGroundBackScale;
};