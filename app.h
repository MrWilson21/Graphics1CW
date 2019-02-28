#pragma once

#include <windows.h>		// Header file for Windows
#include "Image_Loading/nvImage.h"
#include <iostream>
#include <gl\gl.h>			// Header file for the OpenGL32 Library
#include <gl\glu.h>			// Header file for the GLu32 Library
#include <time.h>
#include <string>
#include <vector>
#include <thread> 
#include <chrono>
#include <stdlib.h>

using namespace std;
using namespace std::chrono;

class App
{
	public:
		static bool keys[256];
		static const double degToRad;
		static bool shouldDrawBoundingBoxes;

		static double deltaTime;

		static GLuint loadPNG(char* name, bool repeatX = false, bool repeatY = false);
		static void displayBoundingBox(float x1, float y1, float x2, float y2);

		static void changeToMenuScreen();
		static void changeToWorldScreen();
		static void changeToLoadingScreen();
		static void resetScreenState();
		static void fadeIn();
		static void fadeOut();

		static bool leftPressed;
		static float mouseX;
		static float mouseY;

		static bool isOnMenu;
		static bool isPaused;
		static bool worldIsInPlay;
		static bool isLoadingScreen;

		static bool hasLoaded;
		static bool playButtonPressed;

		static float fadeTransparency;
		static bool isFadingIn;
		static bool isFadingOut;
		static float fadeSpeed;

		typedef struct Point {
			float x;
			float y;
		};
};


