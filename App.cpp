#include "app.h"

bool App::keys[256];
const double App::degToRad = 3.14159265359 / 180.0;
const double App::radToDeg = 180 / 3.14159265359;
double App::deltaTime = 0.0;
bool App::shouldDrawBoundingBoxes = false;
bool App::leftPressed = false;
bool App::escapePressed = false;
float App::mouseX = 0;
float App::mouseY = 0;

bool App::isOnMenu = true;
bool App::isPaused = false;
bool App::worldIsInPlay = false;
bool App::hasLoaded = false;
bool App::isFadingIn = false;
bool App::isLoadingScreen = false;
bool App::isFadingOut = false;
bool App::playButtonPressed = false;
bool App::quitToMenuPressed = false;
bool App::isGameOverScreen = false;
float App::fadeTransparency = 1.0;
float App::fadeSpeed = 3.0;

GLuint App::loadPNG(char* name, bool repeatX, bool repeatY)
{
	// Texture loading object
	nv::Image img;

	GLuint myTextureID;

	// Return true on success
	if (img.loadImageFromFile(name))
	{
		glGenTextures(1, &myTextureID);
		glBindTexture(GL_TEXTURE_2D, myTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img.getInternalFormat(), img.getWidth(), img.getHeight(), 0, img.getFormat(), img.getType(), img.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (repeatX)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		}
		if (repeatY)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		}
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
	}

	else
		MessageBox(NULL, name, "Failed to load texture", MB_OK | MB_ICONINFORMATION);

	return myTextureID;
}

void App::displayBoundingBox(float x1, float y1, float x2, float y2)
{
	if (App::shouldDrawBoundingBoxes)
	{
		glBegin(GL_LINE_STRIP);
		glVertex2f(x1, y2);
		glVertex2f(x2, y2);
		glVertex2f(x2, y1);
		glVertex2f(x1, y1);
		glVertex2f(x1, y2);
		glEnd();
	}
}

void App::fadeIn()
{
	if (App::isFadingIn)
	{
		App::fadeTransparency += App::fadeSpeed * App::deltaTime;
		if (App::fadeTransparency >= 1.0)
		{
			App::fadeTransparency = 1.0;
			App::isFadingIn = false;
		}
	}
}
void App::fadeOut()
{
	if (App::isFadingOut)
	{
		App::fadeTransparency -= App::fadeSpeed * App::deltaTime;
		if (App::fadeTransparency <= 0.0)
		{
			App::fadeTransparency = 0.0;
			App::isFadingOut = false;
		}
	}
}

void App::resetScreenState()
{
	App::isOnMenu = false;
	App::isPaused = false;
	App::isLoadingScreen = false;
	App::isGameOverScreen = false;
	App::worldIsInPlay = false;
	App::isFadingOut = false;
	App::isFadingIn = true;
	App::fadeTransparency = 0.0;
}

void App::changeToGameOverScreen()
{
	App::resetScreenState();
	App::quitToMenuPressed = false;
	App::isGameOverScreen = true;
}

void App::changeToMenuScreen()
{
	App::playButtonPressed = false;
	App::resetScreenState();
	App::isOnMenu = true;
}
void App::changeToWorldScreen()
{
	App::quitToMenuPressed = false;
	App::resetScreenState();
	App::worldIsInPlay = true;
}

void App::changeToLoadingScreen()
{
	App::hasLoaded = false;
	App::resetScreenState();
	App::isLoadingScreen = true;
}