#include "world.h"

World::World()
{
}

void World::init()
{
	player.loadSprites();
	
	//Initial position of camera
	cameraX = player.x + player.colliderWidth / 2;
	cameraY = player.y + player.colliderHeight / 2;

	staticBackground = App::loadPNG("worldBackGround/static.png");
	backGroundFront = App::loadPNG("worldBackGround/front.png");
	backGroundMiddle = App::loadPNG("worldBackGround/middle.png");
	backGroundBack = App::loadPNG("worldBackGround/back.png");

	staticBlocks.push_back(StaticBlock(100, 200, 110,10, "blocks/0.png"));
	staticBlocks.push_back(StaticBlock(210, 200, 40, 10, "blocks/0.png"));

	staticBlocks.push_back(StaticBlock(210, 210, 10, 10, "blocks/0.png"));
	staticBlocks.push_back(StaticBlock(200, 220, 10, 10, "blocks/0.png"));
	staticBlocks.push_back(StaticBlock(190, 230, 10, 10, "blocks/0.png"));
	staticBlocks.push_back(StaticBlock(180, 240, 10, 10, "blocks/0.png"));
	staticBlocks.push_back(StaticBlock(170, 250, 10, 10, "blocks/0.png"));
	staticBlocks.push_back(StaticBlock(160, 260, 10, 10, "blocks/0.png"));
	staticBlocks.push_back(StaticBlock(150, 270, 10, 10, "blocks/0.png"));
	staticBlocks.push_back(StaticBlock(140, 280, 10, 10, "blocks/0.png"));
	
	staticBlocks.push_back(StaticBlock(250, 200, 40, 10, "blocks/0.png"));
	staticBlocks.push_back(StaticBlock(290, 200, 310, 10, "blocks/0.png"));
}

void World::moveCamera(float screenWidth, float screenHeight)
{
	//Calculate player and camera bounding box 
	float playerCentreX = player.x + player.colliderWidth / 2;
	float playerCentreY = player.y + player.colliderHeight / 2;

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
	float offsetX = cameraX * backGroundStaticWidth * backGroundFrontSpeedX;
	float offsetY = cameraY * backGroundFrontSpeedY;

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, backGroundFront);
	glPushMatrix();
	glTranslatef(cameraX, cameraY, 0.0);
	glBegin(GL_POLYGON);
	glTexCoord2f(offsetX,								offsetY + 1.0/backGroundFrontScale);	glVertex2f(-screenWidth/2, screenHeight/2);
	glTexCoord2f(offsetX + 1.0/backGroundFrontScale,	offsetY + 1.0/backGroundFrontScale);	glVertex2f(screenWidth/2, screenHeight/2);
	glTexCoord2f(offsetX + 1.0/backGroundFrontScale,	offsetY);								glVertex2f(screenWidth/2, -screenHeight/2);
	glTexCoord2f(offsetX,								offsetY);								glVertex2f(-screenWidth/2, -screenHeight/2);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void World::update()
{
	player.updatePlayer(staticBlocks);
}

void World::displayPlayerCameraBox(float screenWidth, float screenHeight)
{
	float playerCentreX = player.x + player.colliderWidth / 2;
	float playerCentreY = player.y + player.colliderHeight / 2;
	float topOfPlayerBox = playerCentreY + playerCameraBoxRadiusY * (screenHeight / 100);
	float bottomOfPlayerBox = playerCentreY - playerCameraBoxRadiusY * (screenHeight / 100);
	float rightOfPlayerBox = playerCentreX + playerCameraBoxRadiusX * (screenWidth / 100);
	float leftOfPlayerBox = playerCentreX - playerCameraBoxRadiusX * (screenWidth / 100);

	App::displayBoundingBox(leftOfPlayerBox, bottomOfPlayerBox, rightOfPlayerBox, topOfPlayerBox);
}

void World::display()
{
	for (StaticBlock block : staticBlocks)
	{
		block.display();
	}

	player.displayPlayer();
}
