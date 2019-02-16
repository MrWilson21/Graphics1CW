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
	void initBackGround();

	float worldStartX = 0.0;
	float worldStartY = 0.0;
	float worldSizeX = 500.0;
	float worldSizeY = 500.0;

	float cameraX;
	float cameraY;
	float playerCameraBoxRadiusX = 30;
	float playerCameraBoxRadiusY = 40;

	typedef struct BackGround {
		bool isStatic;
		GLuint texture;
		//Speed the background scolls with the camera
		float scrollSpeedX;
		float scrollSpeedY;
		float scale;
		float aspectRatio;
		//speed that the background scolls on its own (e.g. passing clouds)
		float moveSpeedX;
	};

	BackGround paralaxBackGround[10];
};