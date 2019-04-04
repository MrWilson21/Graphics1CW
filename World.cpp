#include "world.h"

World::World()
{
}

//Destory all entities belonging to world so it can be re intialised
void World::reset()
{
	delete player;
	staticBlocks.clear();
	movingBlocks.clear();
	enemies.clear();
	gems.clear();
	rotatingBlocks.clear();
	particals.clear();
}

void World::init()
{
	worldStartX = -232;
	worldStartY = 0.0;
	worldSizeX = 1673.0;
	worldSizeY = 1000.0;

	leftEdge = worldStartX + 10;
	rightEdge = worldSizeX - 10;
	topEdge = worldSizeY - 10;
	bottomEdge = 30;

	cameraX;
	cameraY;
	playerCameraBoxRadiusX = 30;
	playerCameraBoxRadiusY = 40;

	delayUntilGameEnd = 1.5;
	timeSinceGameEnd = 0.0;
	gameEnding = false;
	gameEnded = false;

	paralaxBackGround[12];

	//Starting area
	player = new Player(740, 100, this);
	staticBlocks.push_back(StaticBlock(720, 90, 60, 10, 10, 10, "blocks/wood.png"));

	//First gem
	staticBlocks.push_back(StaticBlock(720, 160, 60, 10, 10, 10, "blocks/stone.png"));
	gems.push_back(Gem(747.5, 172, 0, "green"));

	//Second gem
	gems.push_back(Gem(747.5, 34, 1, "red"));

	//Elevators
	std::vector<App::Point> p;
	p.push_back(App::Point{ 780, 90 });
	p.push_back(App::Point{ 870, 120 });
	p.push_back(App::Point{ 870, 300 });
	p.push_back(App::Point{ 870, 120 });
	movingBlocks.push_back(MovingBlock(p, 50, 30, 6.875, 30, 6.875, "blocks/movingPlatform.png"));
	staticBlocks.push_back(StaticBlock(810, 300, 60, 10, 10, 10, "blocks/wood.png"));
	enemies.push_back(Enemy(825, 310, this, enemies.size()));
	enemies.push_back(Enemy(835, 310, this, enemies.size()));
	staticBlocks.push_back(StaticBlock(900, 300, 60, 10, 10, 10, "blocks/wood.png"));
	enemies.push_back(Enemy(915, 310, this, enemies.size()));
	enemies.push_back(Enemy(930, 310, this, enemies.size()));

	p.clear();
	p.push_back(App::Point{ 690, 90 });
	p.push_back(App::Point{ 600, 120 });
	p.push_back(App::Point{ 600, 300 });
	p.push_back(App::Point{ 600, 120 });
	movingBlocks.push_back(MovingBlock(p, 50, 30, 6.875, 30, 6.875, "blocks/movingPlatform.png"));
	staticBlocks.push_back(StaticBlock(540, 300, 60, 10, 10, 10, "blocks/wood.png"));
	staticBlocks.push_back(StaticBlock(630, 300, 60, 10, 10, 10, "blocks/wood.png"));

	staticBlocks.push_back(StaticBlock(540, 300, 60, 10, 10, 10, "blocks/wood.png"));
	staticBlocks.push_back(StaticBlock(450, 300, 60, 10, 10, 10, "blocks/wood.png"));
	staticBlocks.push_back(StaticBlock(330, 300, 60, 10, 10, 10, "blocks/wood.png"));
	staticBlocks.push_back(StaticBlock(180, 300, 60, 10, 10, 10, "blocks/wood.png"));

	//Fourth gem
	gems.push_back(Gem(208.5, 312, 3, "purple"));
	staticBlocks.push_back(StaticBlock(100, 300, 60, 10, 10, 10, "blocks/wood.png"));
	enemies.push_back(Enemy(120, 310, this, enemies.size()));
	enemies.push_back(Enemy(140, 310, this, enemies.size()));

	//Hills
	hillSteep(464, 16, 1);
	hillSteep(785, 16, 1);

	//Ramp platforms
	rampPlatformLeft(945, 60, 1.25);
	rampPlatformRight(1075, 140, 1.25);
	rampPlatformLeft(1040, 260, 1.25);

	//Fifth gem
	gems.push_back(Gem(1060, 345, 4, "blue"));

	//Pit of enemies
	float scale = 1;
	float x = 150;
	float y = 16;
	staticBlocks.push_back(StaticBlock(96.09 * scale + x, 47.22 * scale + y, 62.65 * scale, 16 * scale, 16, 16));
	staticBlocks.push_back(StaticBlock(95.7 * scale + x, 48 * scale + y, 64.8 * scale, 16 * scale, 16 * scale, 16 * scale, "blocks/grassBlock.png", false));
	staticBlocks.push_back(StaticBlock(95.7 * scale + x, y, 64.8 * scale, 48.2 * scale, 16 * scale, 16 * scale, "blocks/dirtBlock.png", false));
	staticBlocks.push_back(StaticBlock(160 * scale + x, y, 96 * scale, 64 * scale, 96 * scale, 64 * scale, "blocks/rampLeft.png", false, false, false));
	rotatingBlocks.push_back(RotatingBlock(205.67 * scale + x, 34.41 * scale + y, 109.603 * scale, 10 * scale, 1, 1, -26.5));
	staticBlocks.push_back(StaticBlock(230, 32, 16, 47.1, 16, 16));
	staticBlocks.push_back(StaticBlock(230, 32, 16, 32, 16, 16, "blocks/pillarLeft.png", false, false));
	staticBlocks.push_back(StaticBlock(230, 64, 16, 16, 16, 16, "blocks/pillarTopLeft.png", false, false, false));

	p.clear();
	p.push_back(App::Point{ -136, 45 });
	p.push_back(App::Point{ 200, 45 });
	movingBlocks.push_back(MovingBlock(p, 70, 30, 6.875, 30, 6.875, "blocks/movingPlatform.png"));

	staticBlocks.push_back(StaticBlock(-200, 64, 48, 16, 16, 16, "blocks/grassBlock.png"));
	staticBlocks.push_back(StaticBlock(-200, 16, 48, 48, 16, 16, "blocks/dirtBlock.png"));
	staticBlocks.push_back(StaticBlock(-152, 32, 16, 32, 16, 16, "blocks/pillarRight.png", true, false));
	staticBlocks.push_back(StaticBlock(-152, 64, 16, 16, 16, 16, "blocks/pillarTopRight.png", true, false, false));

	//Third gem
	gems.push_back(Gem(-177.5, 82, 2, "yellow"));

	for (int i = 0; i < 18; i++)
	{
		enemies.push_back(Enemy(-142 + i * 20, 30, this, enemies.size()));
	}

	x = 1250;
	//Right side hill
	staticBlocks.push_back(StaticBlock(x, y, 96 * scale, 64 * scale, 96 * scale, 64 * scale, "blocks/rampRight.png", false, false, false));
	rotatingBlocks.push_back(RotatingBlock(49.17 * scale + x, 34.41 * scale + y, 109.603 * scale, 10 * scale, 1, 1, 26.5));
	staticBlocks.push_back(StaticBlock(96.09 * scale + x, 47.22 * scale + y, 62.65 * scale, 16 * scale, 16, 16));
	staticBlocks.push_back(StaticBlock(95.7 * scale + x, 48 * scale + y, 64.8 * scale, 16 * scale, 16 * scale, 16 * scale, "blocks/grassBlock.png", false));
	staticBlocks.push_back(StaticBlock(95.7 * scale + x, y, 64.8 * scale, 48.2 * scale, 16 * scale, 16 * scale, "blocks/dirtBlock.png", false));

	//Shuffle blocks
	shuffleBlocks(worldStartX + worldSizeX - 62, 120, worldStartX + worldSizeX - 10, 50, 6, 10);
	//Sixth gem
	gems.push_back(Gem(worldStartX + worldSizeX - 44, 382, 5, "orange"));

	//Walls
	staticBlocks.push_back(StaticBlock(worldStartX + 16, 0, 16, worldSizeY, 16, 16, "blocks/wall.png", true , false, true));
	staticBlocks.push_back(StaticBlock(worldStartX, 0, 16.2, worldSizeY, 16, 16, "blocks/wallFill.png"));
	staticBlocks.push_back(StaticBlock(worldStartX + worldSizeX - 32, 0, 16, worldSizeY, 16, 16, "blocks/wallLeft.png", true, false, true));
	staticBlocks.push_back(StaticBlock(worldStartX + worldSizeX - 16.2, 0, 16.2, worldSizeY, 16, 16, "blocks/wallFill.png"));

	//Ground
	staticBlocks.push_back(StaticBlock(worldStartX, 16, worldSizeX, 16, 16, 16, "blocks/grassBlock.png"));
	staticBlocks.push_back(StaticBlock(worldStartX, 0, worldSizeX, 16.2, 16, 16, "blocks/dirtBlock.png"));

	player->loadSprites();
	initBackGround();
	
	//Initial position of camera
	cameraX = player->x + player->colliderWidth / 2;
	cameraY = player->y + player->colliderHeight / 2;
	
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i].loadSprites();
	}
}

//Width 256, height 62.65
void World::hillSteep(float x, float y, float scale)
{
	staticBlocks.push_back(StaticBlock(x, y, 96 * scale, 64 * scale, 96 * scale, 64 * scale, "blocks/rampRight.png", false, false, false));
	rotatingBlocks.push_back(RotatingBlock(49.17 * scale + x, 34.41 * scale + y, 109.603 * scale, 10 * scale, 1, 1, 26.5));
	staticBlocks.push_back(StaticBlock(96.09 * scale + x, 47.22 * scale + y, 62.65 * scale, 16 * scale, 16, 16));
	staticBlocks.push_back(StaticBlock(95.7 * scale + x, 48 * scale + y, 64.8 * scale, 16 * scale, 16 * scale, 16 * scale, "blocks/grassBlock.png", false));
	staticBlocks.push_back(StaticBlock(95.7 * scale + x, y, 64.8 * scale, 48.2 * scale, 16 * scale, 16 * scale, "blocks/dirtBlock.png", false));
	staticBlocks.push_back(StaticBlock(160 * scale + x, y, 96 * scale, 64 * scale, 96 * scale, 64 * scale, "blocks/rampLeft.png", false, false, false));
	rotatingBlocks.push_back(RotatingBlock(205.67 * scale + x, 34.41 * scale + y, 109.603 * scale, 10 * scale, 1, 1, -26.5));
}

//Width 156.09, height 65.39
void World::rampPlatformRight(float x, float y, float scale)
{
	staticBlocks.push_back(StaticBlock		(x					, y					, 40 * scale		, 10 * scale	, 10 * scale , 10 * scale		, "blocks/stone.png"));
	enemies.push_back(Enemy(x + 15 * scale, y + 10 * scale, this, enemies.size()));
	rotatingBlocks.push_back(RotatingBlock	(78.04 * scale + x	, 32.73 * scale + y	, 100 * scale		, 10 * scale	, 10 * scale , 10 * scale, 35	, "blocks/stone.png"));
	staticBlocks.push_back(StaticBlock		(116.09 * scale + x	, 55.39 * scale + y	, 40 * scale		, 10 * scale	, 10 * scale , 10 * scale		, "blocks/stone.png"));
	enemies.push_back(Enemy(x + 131 * scale, y + 66 * scale, this, enemies.size()));
}

void World::shuffleBlocks(float x, float y, float x2, float y2, int noOfBlocks, float speed)
{
	float distanceX = (x2 - x) / noOfBlocks;
	float distanceY = y2;

	for (int i = 0; i < noOfBlocks; i++)
	{
		std::vector<App::Point> p;
		p.push_back(App::Point{ x + (noOfBlocks-i-1)*distanceX, y + i*distanceY });
		p.push_back(App::Point{ x2, y + i*distanceY });
		p.push_back(App::Point{ x, y + i*distanceY });
		movingBlocks.push_back(MovingBlock(p, speed, 30, 10, 10, 10, "blocks/0.png"));
	}
}

//Width 156.09, height 65.39
void World::rampPlatformLeft(float x, float y, float scale)
{
	staticBlocks.push_back(StaticBlock(x, 55.39 * scale + y, 40 * scale, 10 * scale, 10 * scale, 10 * scale, "blocks/stone.png"));
	enemies.push_back(Enemy(x + 15 * scale, y + 66 * scale, this, enemies.size()));
	rotatingBlocks.push_back(RotatingBlock(78.04 * scale + x, 32.73 * scale + y, 100 * scale, 10 * scale, 10 * scale, 10 * scale, -35, "blocks/stone.png"));
	staticBlocks.push_back(StaticBlock(116.09 * scale + x, y, 40 * scale, 10 * scale, 10 * scale, 10 * scale, "blocks/stone.png"));
	enemies.push_back(Enemy(x + 131 * scale, y + 10 * scale, this, enemies.size()));
}

void World::signalGameEnd()
{
	gameEnding = true;
}

void World::createPartical(float x1, float y1, float width, float height, float scale, string sprite)
{
	particals.push_back(Partical(x1 - width * scale/2, y1 - height * scale/2, width, height, scale, sprite, this));
}

//Define background parameters and textures here and intitialise them
void World::initBackGround()
{
	float worldBackground = 1.5;
	paralaxBackGround[0].isStatic = true;
	paralaxBackGround[0].texture = App::loadPNG("worldBackGround/static.png", true, false);
	paralaxBackGround[0].scrollSpeedX = 0;
	paralaxBackGround[0].scrollSpeedY = 0;
	paralaxBackGround[0].aspectRatio = 0;
	paralaxBackGround[0].moveSpeedX = 0;
	paralaxBackGround[0].scale = 1;

	paralaxBackGround[1].isStatic = false;
	paralaxBackGround[1].texture = App::loadPNG("worldBackGround/hugeClouds.png", true, false);
	paralaxBackGround[1].scrollSpeedX = 0.05;
	paralaxBackGround[1].scrollSpeedY = 0.15;
	paralaxBackGround[1].aspectRatio = 2048.0 / 1546.0;
	paralaxBackGround[1].moveSpeedX = 0.1;
	paralaxBackGround[1].scale = 100 * worldBackground;

	paralaxBackGround[2].isStatic = false;
	paralaxBackGround[2].texture = App::loadPNG("worldBackGround/distantClouds.png", true, false);
	paralaxBackGround[2].scrollSpeedX = 0.05;
	paralaxBackGround[2].scrollSpeedY = 0.15;
	paralaxBackGround[2].aspectRatio = 2048.0 / 1546.0;
	paralaxBackGround[2].moveSpeedX = 0;
	paralaxBackGround[2].scale = 100 * worldBackground;

	paralaxBackGround[3].isStatic = false;
	paralaxBackGround[3].texture = App::loadPNG("worldBackGround/distantClouds1.png", true, false);
	paralaxBackGround[3].scrollSpeedX = 0.1;
	paralaxBackGround[3].scrollSpeedY = 0.2;
	paralaxBackGround[3].aspectRatio = 2048.0 / 1546.0;
	paralaxBackGround[3].moveSpeedX = 0;
	paralaxBackGround[3].scale = 100 * worldBackground;

	paralaxBackGround[4].isStatic = false;
	paralaxBackGround[4].texture = App::loadPNG("worldBackGround/clouds.png", true, false);
	paralaxBackGround[4].scrollSpeedX = 0.15;
	paralaxBackGround[4].scrollSpeedY = 0.25;
	paralaxBackGround[4].aspectRatio = 2048.0 / 1546.0;
	paralaxBackGround[4].moveSpeedX = 0;
	paralaxBackGround[4].scale = 100 * worldBackground;

	paralaxBackGround[5].isStatic = false;
	paralaxBackGround[5].texture = App::loadPNG("worldBackGround/rainbow.png", false, false);
	paralaxBackGround[5].scrollSpeedX = 0.14;
	paralaxBackGround[5].scrollSpeedY = 0.1;
	paralaxBackGround[5].aspectRatio = 1220.0 / 693.0;
	paralaxBackGround[5].moveSpeedX = 0;
	paralaxBackGround[5].scale = 200 * worldBackground;

	paralaxBackGround[6].isStatic = false;
	paralaxBackGround[6].texture = App::loadPNG("worldBackGround/hill2.png", true, false);
	paralaxBackGround[6].scrollSpeedX = 0.13;
	paralaxBackGround[6].scrollSpeedY = 0.27;
	paralaxBackGround[6].aspectRatio = 2048.0 / 1546.0;
	paralaxBackGround[6].moveSpeedX = 0;
	paralaxBackGround[6].scale = 100 * worldBackground;

	paralaxBackGround[7].isStatic = false;
	paralaxBackGround[7].texture = App::loadPNG("worldBackGround/hill1.png", true, false);
	paralaxBackGround[7].scrollSpeedX = 0.2;
	paralaxBackGround[7].scrollSpeedY = 0.35;
	paralaxBackGround[7].aspectRatio = 2048.0 / 1546.0;
	paralaxBackGround[7].moveSpeedX = 0;
	paralaxBackGround[7].scale = 100 * worldBackground;

	paralaxBackGround[8].isStatic = false;
	paralaxBackGround[8].texture = App::loadPNG("worldBackGround/bushes.png", true, false);
	paralaxBackGround[8].scrollSpeedX = 0.25;
	paralaxBackGround[8].scrollSpeedY = 0.6;
	paralaxBackGround[8].aspectRatio = 2048.0 / 1546.0;
	paralaxBackGround[8].moveSpeedX = 0;
	paralaxBackGround[8].scale = 100 * worldBackground;

	paralaxBackGround[9].isStatic = false;
	paralaxBackGround[9].texture = App::loadPNG("worldBackGround/distantTrees.png", true, false);
	paralaxBackGround[9].scrollSpeedX = 0.3;
	paralaxBackGround[9].scrollSpeedY = 0.7;
	paralaxBackGround[9].aspectRatio = 2048.0 / 1546.0;
	paralaxBackGround[9].moveSpeedX = 0;
	paralaxBackGround[9].scale = 100 * worldBackground;

	paralaxBackGround[10].isStatic = false;
	paralaxBackGround[10].texture = App::loadPNG("worldBackGround/treesAndBushes.png", true, false);
	paralaxBackGround[10].scrollSpeedX = 0.8;
	paralaxBackGround[10].scrollSpeedY = 0.8;
	paralaxBackGround[10].aspectRatio = 2048.0 / 1546.0;
	paralaxBackGround[10].moveSpeedX = 0;
	paralaxBackGround[10].scale = 100 * worldBackground;

	paralaxBackGround[11].isStatic = false;
	paralaxBackGround[11].texture = App::loadPNG("worldBackGround/ground.png", true, false);
	paralaxBackGround[11].scrollSpeedX = 1;
	paralaxBackGround[11].scrollSpeedY = 1;
	paralaxBackGround[11].aspectRatio = 2048.0/1546.0;
	paralaxBackGround[11].moveSpeedX = 0;
	paralaxBackGround[11].scale = 100 * worldBackground;
}

void World::moveCamera(float screenWidth, float screenHeight)
{
	//Calculate player and camera bounding box 
	float playerCentreX = player->x + player->colliderWidth / 2;
	float playerCentreY = player->y + player->colliderHeight / 2;

	float topOfCameraBox = cameraY + screenHeight / 2;
	float bottomOfCameraBox = cameraY - screenHeight / 2;
	float rightOfCameraBox = cameraX + screenWidth / 2;
	float leftOfCameraBox = cameraX - screenWidth / 2;

	float topOfPlayerBox = playerCentreY + playerCameraBoxRadiusY * (screenHeight / 100);
	float bottomOfPlayerBox = playerCentreY - playerCameraBoxRadiusY * (screenHeight / 100);
	float rightOfPlayerBox = playerCentreX + playerCameraBoxRadiusX * (screenWidth / 100);
	float leftOfPlayerBox = playerCentreX - playerCameraBoxRadiusX * (screenWidth / 100);

	//Move camera with the player if player camera box goes outside camera view 

	if (rightOfPlayerBox > rightOfCameraBox)
	{
		cameraX += rightOfPlayerBox - rightOfCameraBox;
	}
	else if (leftOfPlayerBox < leftOfCameraBox)
	{
		cameraX += leftOfPlayerBox - leftOfCameraBox;
	}

	if (topOfPlayerBox > topOfCameraBox)
	{
		cameraY += topOfPlayerBox - topOfCameraBox;
	}
	else if (bottomOfPlayerBox < bottomOfCameraBox)
	{
		cameraY += bottomOfPlayerBox - bottomOfCameraBox;
	}

	//Recalculate camera bounding box position
	topOfCameraBox = cameraY + screenHeight / 2;
	bottomOfCameraBox = cameraY - screenHeight / 2;
	rightOfCameraBox = cameraX + screenWidth / 2;
	leftOfCameraBox = cameraX - screenWidth / 2;

	//Check if camera is outside world bouding box and move back in
	if (rightOfCameraBox > worldStartX + worldSizeX)
	{
		cameraX = worldStartX + worldSizeX - screenWidth / 2;
	}
	else if (leftOfCameraBox < worldStartX)
	{
		cameraX = worldStartX + screenWidth / 2;
	}
	if (topOfCameraBox > worldStartY + worldSizeY)
	{
		cameraY = worldStartY + worldSizeY - screenHeight / 2;
	}
	else if (bottomOfCameraBox < worldStartY)
	{
		cameraY = worldStartY + screenHeight / 2;
	}

	//Do final translation to display actual camera movement
	glTranslatef(-cameraX + screenWidth / 2, -cameraY + screenHeight / 2, 0);

	if (App::shouldDrawBoundingBoxes)
	{
		App::displayBoundingBox(leftOfPlayerBox, bottomOfPlayerBox, rightOfPlayerBox, topOfPlayerBox);
	}
}

void World::displayBackground(float screenWidth, float screenHeight)
{
	float screenAspectRatio = screenWidth / screenHeight;

	for (int i = 0; i < 12; i++)
	{
		if (paralaxBackGround[i].isStatic)
		{
			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glBindTexture(GL_TEXTURE_2D, paralaxBackGround[i].texture);
			glPushMatrix();
			glTranslatef(cameraX, cameraY, 0.0);
			glBegin(GL_POLYGON);
			glTexCoord2f(0, 1);	glVertex2f(-screenWidth / 2, screenHeight / 2);
			glTexCoord2f(1, 1);	glVertex2f(screenWidth / 2, screenHeight / 2);
			glTexCoord2f(1, 0);	glVertex2f(screenWidth / 2, -screenHeight / 2);
			glTexCoord2f(0, 0);	glVertex2f(-screenWidth / 2, -screenHeight / 2);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}
		else
		{
			float textureWidth = 1/((paralaxBackGround[i].scale * paralaxBackGround[i].aspectRatio) / screenWidth);
			float offsetX = paralaxBackGround[i].scrollSpeedX * ((cameraX - (screenWidth / 2)) / (paralaxBackGround[i].scale * paralaxBackGround[i].aspectRatio));

			float textureHeight = 1/(paralaxBackGround[i].scale / screenHeight);
			float offsetY = paralaxBackGround[i].scrollSpeedY * (((cameraY - (screenHeight / 2)) / paralaxBackGround[i].scale));

			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glBindTexture(GL_TEXTURE_2D, paralaxBackGround[i].texture);
			glPushMatrix();
			glTranslatef(cameraX, cameraY, 0.0);
			glBegin(GL_POLYGON);
			glTexCoord2f(offsetX,				 offsetY + textureHeight);	glVertex2f(-screenWidth/2, screenHeight/2);
			glTexCoord2f(offsetX + textureWidth, offsetY + textureHeight);	glVertex2f(screenWidth/2, screenHeight/2);
			glTexCoord2f(offsetX + textureWidth, offsetY);					glVertex2f(screenWidth/2, -screenHeight/2);
			glTexCoord2f(offsetX,				 offsetY);					glVertex2f(-screenWidth/2, -screenHeight/2);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}
	}
}

void World::update()
{
	if (enemies.size() == 0)
	{
		signalGameEnd();
	}

	for (int i=0; i < movingBlocks.size(); i++)
	{
		movingBlocks[i].move();
	}
	
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i].update();
	}

	for (int i = 0; i < gems.size(); i++)
	{
		gems[i].update(player);
	}

	for (int i = 0; i < particals.size(); i++)
	{
		particals[i].update();
	}

	if (gameEnding)
	{
		timeSinceGameEnd += App::deltaTime;
		App::fadeTransparency -= App::deltaTime / delayUntilGameEnd;
		if (timeSinceGameEnd > delayUntilGameEnd)
		{
			gameEnded = true;
			App::fadeTransparency = 0.0;
		}
	}

	player->update();
}

void World::displayPlayerCameraBox(float screenWidth, float screenHeight)
{
	float playerCentreX = player->x + player->colliderWidth / 2;
	float playerCentreY = player->y + player->colliderHeight / 2;
	float topOfPlayerBox = playerCentreY + playerCameraBoxRadiusY * (screenHeight / 100);
	float bottomOfPlayerBox = playerCentreY - playerCameraBoxRadiusY * (screenHeight / 100);
	float rightOfPlayerBox = playerCentreX + playerCameraBoxRadiusX * (screenWidth / 100);
	float leftOfPlayerBox = playerCentreX - playerCameraBoxRadiusX * (screenWidth / 100);

	App::displayBoundingBox(leftOfPlayerBox, bottomOfPlayerBox, rightOfPlayerBox, topOfPlayerBox);
}

void World::displayWorldBoundaries()
{
	App::displayBoundingBox(rightEdge, 0, 100 + rightEdge, worldSizeY);
	App::displayBoundingBox(leftEdge - 100, 0, leftEdge, worldSizeY);
	App::displayBoundingBox(0, topEdge, worldSizeX, topEdge + 100);
	App::displayBoundingBox(0, bottomEdge - 100, worldSizeX, bottomEdge);
}

void World::display()
{
	for (int i = 0; i < rotatingBlocks.size(); i++)
	{
		rotatingBlocks[i].x += App::deltaTime * -1 * App::keys[VK_LEFT];
		rotatingBlocks[i].x += App::deltaTime * 1 * App::keys[VK_RIGHT];
		rotatingBlocks[i].y += App::deltaTime * 1 * App::keys[VK_UP];
		rotatingBlocks[i].y += App::deltaTime * -1 * App::keys[VK_DOWN];
		rotatingBlocks[i].width += App::deltaTime * -1 * App::keys[VK_OEM_MINUS];
		rotatingBlocks[i].width += App::deltaTime * 1 * App::keys[VK_OEM_PLUS];
		rotatingBlocks[i].display();
	}

	for (int i = 0; i < movingBlocks.size(); i++)
	{
		movingBlocks[i].display();
	}

	for (int i = 0; i < staticBlocks.size(); i++)
	{
		staticBlocks[i].display();
	}

	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i].display();
	}

	for (int i = 0; i < gems.size(); i++)
	{
		if (!gems[i].collected)
		{
			gems[i].display();
		}
	}

	for (int i = 0; i < particals.size(); i++)
	{
		particals[i].display();
	}

	player->display();
}
