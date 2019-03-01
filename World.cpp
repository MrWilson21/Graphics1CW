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
}

void World::init()
{
	worldStartX = 0.0;
	worldStartY = 0.0;
	worldSizeX = 500.0;
	worldSizeY = 500.0;

	leftEdge = 10;
	rightEdge = worldSizeX - 10;
	topEdge = worldSizeY - 10;
	bottomEdge = 20;

	cameraX;
	cameraY;
	playerCameraBoxRadiusX = 30;
	playerCameraBoxRadiusY = 40;

	delayUntilGameEnd = 1.5;
	timeSinceGameEnd = 0.0;
	gameEnding = false;
	gameEnded = false;

	paralaxBackGround[11];

	player = new Player(30, 90, this);

	player->loadSprites();

	initBackGround();
	
	//Initial position of camera
	cameraX = player->x + player->colliderWidth / 2;
	cameraY = player->y + player->colliderHeight / 2;

	//staticBlocks.push_back(StaticBlock(30, 45, 150, 20, "blocks/0.png"));
	//staticBlocks.push_back(StaticBlock(30, 65, 100, 10, 10, 10, "blocks/0.png"));
	//staticBlocks.push_back(StaticBlock(130, 0, 10, 40, 10, 10, "blocks/0.png"));
	//staticBlocks.push_back(StaticBlock(160, 100, 50, 10, "blocks/0.png"));
	staticBlocks.push_back(StaticBlock(250, 0, 10, 60, 10, 10, "blocks/0.png"));

	rotatingBlocks.push_back(RotatingBlock(40, 30, 40, 40, 10, 10, 0, "blocks/0.png"));

	for(int i = 0; i < 1; i++)
	{
		enemies.push_back(Enemy(300, 30, this, i));
	}
	
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i].loadSprites();
	}

	gems.push_back(Gem(80, 80, 0, "green"));
	gems.push_back(Gem(80, 80, 1, "red"));
	gems.push_back(Gem(80, 80, 2,"yellow"));
	gems.push_back(Gem(80, 80, 3, "purple"));
	gems.push_back(Gem(80, 80, 4, "blue"));
	gems.push_back(Gem(80, 80, 5, "orange"));

	std::vector<App::Point> p;
	p.push_back(App::Point{ 30,10 });
	p.push_back(App::Point{ 100,10 });
	p.push_back(App::Point{ 100,100 });
	p.push_back(App::Point{ 50,30 });
	//movingBlocks.push_back(MovingBlock(p, 350, 30, 20, 10, 10, "blocks/0.png"));
}

void World::signalGameEnd()
{
	gameEnding = true;
}

//Define background parameters and textures here and intitialise them
void World::initBackGround()
{
	paralaxBackGround[0].isStatic = true;
	paralaxBackGround[0].texture = App::loadPNG("worldBackGround/static.png", true, false);
	paralaxBackGround[0].scrollSpeedX = 0;
	paralaxBackGround[0].scrollSpeedY = 0;
	paralaxBackGround[0].aspectRatio = 0;
	paralaxBackGround[0].moveSpeedX = 0;
	paralaxBackGround[0].scale = 0;

	paralaxBackGround[1].isStatic = false;
	paralaxBackGround[1].texture = App::loadPNG("worldBackGround/hugeClouds.png", true, false);
	paralaxBackGround[1].scrollSpeedX = 0.05;
	paralaxBackGround[1].scrollSpeedY = 0.15;
	paralaxBackGround[1].aspectRatio = 2048.0 / 1546.0;
	paralaxBackGround[1].moveSpeedX = 0.1;
	paralaxBackGround[1].scale = 100;

	paralaxBackGround[2].isStatic = false;
	paralaxBackGround[2].texture = App::loadPNG("worldBackGround/distantClouds.png", true, false);
	paralaxBackGround[2].scrollSpeedX = 0.05;
	paralaxBackGround[2].scrollSpeedY = 0.15;
	paralaxBackGround[2].aspectRatio = 2048.0 / 1546.0;
	paralaxBackGround[2].moveSpeedX = 0;
	paralaxBackGround[2].scale = 100;

	paralaxBackGround[3].isStatic = false;
	paralaxBackGround[3].texture = App::loadPNG("worldBackGround/distantClouds1.png", true, false);
	paralaxBackGround[3].scrollSpeedX = 0.1;
	paralaxBackGround[3].scrollSpeedY = 0.2;
	paralaxBackGround[3].aspectRatio = 2048.0 / 1546.0;
	paralaxBackGround[3].moveSpeedX = 0;
	paralaxBackGround[3].scale = 100;

	paralaxBackGround[4].isStatic = false;
	paralaxBackGround[4].texture = App::loadPNG("worldBackGround/clouds.png", true, false);
	paralaxBackGround[4].scrollSpeedX = 0.15;
	paralaxBackGround[4].scrollSpeedY = 0.25;
	paralaxBackGround[4].aspectRatio = 2048.0 / 1546.0;
	paralaxBackGround[4].moveSpeedX = 0;
	paralaxBackGround[4].scale = 100;

	paralaxBackGround[5].isStatic = false;
	paralaxBackGround[5].texture = App::loadPNG("worldBackGround/hill2.png", true, false);
	paralaxBackGround[5].scrollSpeedX = 0.13;
	paralaxBackGround[5].scrollSpeedY = 0.27;
	paralaxBackGround[5].aspectRatio = 2048.0 / 1546.0;
	paralaxBackGround[5].moveSpeedX = 0;
	paralaxBackGround[5].scale = 100;

	paralaxBackGround[6].isStatic = false;
	paralaxBackGround[6].texture = App::loadPNG("worldBackGround/hill1.png", true, false);
	paralaxBackGround[6].scrollSpeedX = 0.2;
	paralaxBackGround[6].scrollSpeedY = 0.35;
	paralaxBackGround[6].aspectRatio = 2048.0 / 1546.0;
	paralaxBackGround[6].moveSpeedX = 0;
	paralaxBackGround[6].scale = 100;

	paralaxBackGround[7].isStatic = false;
	paralaxBackGround[7].texture = App::loadPNG("worldBackGround/bushes.png", true, false);
	paralaxBackGround[7].scrollSpeedX = 0.25;
	paralaxBackGround[7].scrollSpeedY = 0.6;
	paralaxBackGround[7].aspectRatio = 2048.0 / 1546.0;
	paralaxBackGround[7].moveSpeedX = 0;
	paralaxBackGround[7].scale = 100;

	paralaxBackGround[8].isStatic = false;
	paralaxBackGround[8].texture = App::loadPNG("worldBackGround/distantTrees.png", true, false);
	paralaxBackGround[8].scrollSpeedX = 0.3;
	paralaxBackGround[8].scrollSpeedY = 0.7;
	paralaxBackGround[8].aspectRatio = 2048.0 / 1546.0;
	paralaxBackGround[8].moveSpeedX = 0;
	paralaxBackGround[8].scale = 100;

	paralaxBackGround[9].isStatic = false;
	paralaxBackGround[9].texture = App::loadPNG("worldBackGround/treesAndBushes.png", true, false);
	paralaxBackGround[9].scrollSpeedX = 0.8;
	paralaxBackGround[9].scrollSpeedY = 0.8;
	paralaxBackGround[9].aspectRatio = 2048.0 / 1546.0;
	paralaxBackGround[9].moveSpeedX = 0;
	paralaxBackGround[9].scale = 100;

	paralaxBackGround[10].isStatic = false;
	paralaxBackGround[10].texture = App::loadPNG("worldBackGround/ground.png", true, false);
	paralaxBackGround[10].scrollSpeedX = 1;
	paralaxBackGround[10].scrollSpeedY = 1;
	paralaxBackGround[10].aspectRatio = 2048.0/1546.0;
	paralaxBackGround[10].moveSpeedX = 0;
	paralaxBackGround[10].scale = 100;
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

	for (int i = 0; i < 11; i++)
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
	for (int i = 0; i < staticBlocks.size(); i++)
	{
		staticBlocks[i].display();
	}

	for (int i = 0; i < movingBlocks.size(); i++)
	{
		movingBlocks[i].display();
	}

	for (int i = 0; i < rotatingBlocks.size(); i++)
	{
		rotatingBlocks[i].display();
		rotatingBlocks[i].rotation += App::deltaTime * App::keys[VK_NUMPAD2] * 50;
		rotatingBlocks[i].rotation -= App::deltaTime * App::keys[VK_NUMPAD1] * 50;
		rotatingBlocks[i].calculatePoints();
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

	player->display();
}
