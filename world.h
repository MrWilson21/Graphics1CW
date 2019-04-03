#pragma once
#include "app.h"
#include "staticBlock.h"
#include "movingblock.h"
#include "player.h"
#include "enemy.h"
#include "gem.h"
#include "rotatingBlock.h"
#include "partical.h"

class World
{
public:
	World();
	void init();
	void reset();
	void update();
	void display();
	void moveCamera(float screenWidth, float screenHeight);
	void displayBackground(float screenWidth, float screenHeight);
	void displayPlayerCameraBox(float screenWidth, float screenHeight);
	void displayWorldBoundaries();
	void signalGameEnd();
	void createPartical(float x1, float y1, float width, float height, float scale, string sprite);

	Player* player;
	vector<StaticBlock> staticBlocks;	
	vector<MovingBlock> movingBlocks;
	vector<RotatingBlock> rotatingBlocks;
	vector<Enemy> enemies;
	vector<Gem> gems;
	vector<Partical> particals;
	
	float worldStartX;
	float worldStartY;
	float worldSizeX;
	float worldSizeY;

	float leftEdge;
	float rightEdge;
	float topEdge;
	float bottomEdge;

	bool gameEnded;

private:
	void initBackGround();

	void hillSteep(float x, float y, float scale);
	void rampPlatformLeft(float x, float y, float scale);
	void rampPlatformRight(float x, float y, float scale);

	float cameraX;
	float cameraY;
	float playerCameraBoxRadiusX;
	float playerCameraBoxRadiusY;

	float delayUntilGameEnd;
	float timeSinceGameEnd;
	bool gameEnding;

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
	BackGround paralaxBackGround[12];
};