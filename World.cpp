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


//Define background parameters and textures here and intitialise them
void World::initBackGround()
{
	paralaxBackGround[0].isStatic = true;
	paralaxBackGround[0].texture = App::loadPNG("worldBackGround/static.png");
	paralaxBackGround[0].scrollSpeedX = 0;
	paralaxBackGround[0].scrollSpeedY = 0;
	paralaxBackGround[0].aspectRatio = 0;
	paralaxBackGround[0].moveSpeedX = 0;
	paralaxBackGround[0].scale = 0;

	paralaxBackGround[1].isStatic = false;
	paralaxBackGround[1].texture = App::loadPNG("worldBackGround/static.png");
	paralaxBackGround[1].scrollSpeedX = 1;
	paralaxBackGround[1].scrollSpeedY = 1;
	paralaxBackGround[1].aspectRatio = 2048.0/1546.0;
	paralaxBackGround[1].moveSpeedX = 0;
	paralaxBackGround[1].scale = 1;

	/*paralaxBackGround[2].isStatic = true;
	paralaxBackGround[2].texture = App::loadPNG("worldBackGround/static.png");
	paralaxBackGround[2].scrollSpeedX = 0;
	paralaxBackGround[2].scrollSpeedY = 0;
	paralaxBackGround[2].aspectRatio = 0;
	paralaxBackGround[2].moveSpeedX = 0;
	paralaxBackGround[2].scale = 0;

	paralaxBackGround[3].isStatic = true;
	paralaxBackGround[3].texture = App::loadPNG("worldBackGround/static.png");
	paralaxBackGround[3].scrollSpeedX = 0;
	paralaxBackGround[3].scrollSpeedY = 0;
	paralaxBackGround[3].aspectRatio = 0;
	paralaxBackGround[3].moveSpeedX = 0;
	paralaxBackGround[3].scale = 0;

	paralaxBackGround[4].isStatic = true;
	paralaxBackGround[4].texture = App::loadPNG("worldBackGround/static.png");
	paralaxBackGround[4].scrollSpeedX = 0;
	paralaxBackGround[4].scrollSpeedY = 0;
	paralaxBackGround[4].aspectRatio = 0;
	paralaxBackGround[4].moveSpeedX = 0;
	paralaxBackGround[4].scale = 0;

	paralaxBackGround[5].isStatic = true;
	paralaxBackGround[5].texture = App::loadPNG("worldBackGround/static.png");
	paralaxBackGround[5].scrollSpeedX = 0;
	paralaxBackGround[5].scrollSpeedY = 0;
	paralaxBackGround[5].aspectRatio = 0;
	paralaxBackGround[5].moveSpeedX = 0;
	paralaxBackGround[5].scale = 0;

	paralaxBackGround[6].isStatic = true;
	paralaxBackGround[6].texture = App::loadPNG("worldBackGround/static.png");
	paralaxBackGround[6].scrollSpeedX = 0;
	paralaxBackGround[6].scrollSpeedY = 0;
	paralaxBackGround[6].aspectRatio = 0;
	paralaxBackGround[6].moveSpeedX = 0;
	paralaxBackGround[6].scale = 0;

	paralaxBackGround[6].isStatic = true;
	paralaxBackGround[6].texture = App::loadPNG("worldBackGround/static.png");
	paralaxBackGround[6].scrollSpeedX = 0;
	paralaxBackGround[6].scrollSpeedY = 0;
	paralaxBackGround[6].aspectRatio = 0;
	paralaxBackGround[6].moveSpeedX = 0;
	paralaxBackGround[6].scale = 0;

	paralaxBackGround[7].isStatic = true;
	paralaxBackGround[7].texture = App::loadPNG("worldBackGround/static.png");
	paralaxBackGround[7].scrollSpeedX = 0;
	paralaxBackGround[7].scrollSpeedY = 0;
	paralaxBackGround[7].aspectRatio = 0;
	paralaxBackGround[7].moveSpeedX = 0;
	paralaxBackGround[7].scale = 0;

	paralaxBackGround[8].isStatic = true;
	paralaxBackGround[8].texture = App::loadPNG("worldBackGround/static.png");
	paralaxBackGround[8].scrollSpeedX = 0;
	paralaxBackGround[8].scrollSpeedY = 0;
	paralaxBackGround[8].aspectRatio = 0;
	paralaxBackGround[8].moveSpeedX = 0;
	paralaxBackGround[8].scale = 0;

	paralaxBackGround[9].isStatic = true;
	paralaxBackGround[9].texture = App::loadPNG("worldBackGround/static.png");
	paralaxBackGround[9].scrollSpeedX = 0;
	paralaxBackGround[9].scrollSpeedY = 0;
	paralaxBackGround[9].aspectRatio = 0;
	paralaxBackGround[9].moveSpeedX = 0;
	paralaxBackGround[9].scale = 0;*/
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
	for (int i = 0; i < 9; i++)
	{
		if (paralaxBackGround[i].isStatic)
		{

		}
		else
		{
			float offsetX = cameraX * paralaxBackGround[i].aspectRatio * paralaxBackGround[i].scrollSpeedX;
			float offsetY = cameraY * paralaxBackGround[i].scrollSpeedY;

			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glBindTexture(GL_TEXTURE_2D, paralaxBackGround[i].texture);
			glPushMatrix();
			glTranslatef(cameraX, cameraY, 0.0);
			glBegin(GL_POLYGON);
			glTexCoord2f(offsetX,									offsetY + 1.0/paralaxBackGround[i].scale);	glVertex2f(-screenWidth/2, screenHeight/2);
			glTexCoord2f(offsetX + 1.0/paralaxBackGround[i].scale,	offsetY + 1.0/ paralaxBackGround[i].scale);	glVertex2f(screenWidth/2, screenHeight/2);
			glTexCoord2f(offsetX + 1.0/ paralaxBackGround[i].scale,	offsetY);									glVertex2f(screenWidth/2, -screenHeight/2);
			glTexCoord2f(offsetX,									offsetY);									glVertex2f(-screenWidth/2, -screenHeight/2);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}
	}
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
