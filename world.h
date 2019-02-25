#pragma once
#include "app.h"
#include "staticBlock.h"
#include "movingblock.h"
#include "player.h"
#include "enemy.h"
#include "gem.h"

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
	void displayWorldBoundaries();

	Player* player;
	vector<StaticBlock> staticBlocks;	
	vector<MovingBlock> movingBlocks;
	vector<Enemy> enemies;

	vector<Gem> gems;
	
	float worldStartX;
	float worldStartY;
	float worldSizeX;
	float worldSizeY;

	float leftEdge;
	float rightEdge;
	float topEdge;
	float bottomEdge;



private:
	void initBackGround();

	float cameraX;
	float cameraY;
	float playerCameraBoxRadiusX;
	float playerCameraBoxRadiusY;

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
	BackGround paralaxBackGround[11];
};