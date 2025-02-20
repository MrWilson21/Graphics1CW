#pragma once

#include <windows.h>		// Header file for Windows
#include "Image_Loading/nvImage.h"
#include <iostream>
#include <fstream>
#include <gl\gl.h>			// Header file for the OpenGL32 Library
#include <gl\glu.h>			// Header file for the GLu32 Library
#include <time.h>
#include <string>
#include <vector>
#include <thread> 
#include <chrono>
#include <stdlib.h>
#include <algorithm>
#include "GL/glew.h"
#include "freetype2.h"

using namespace std;
using namespace std::chrono;
using namespace freetype;

class App
{
	public:
		static bool keys[256];
		static const double degToRad;
		static const double radToDeg;
		static bool shouldDrawBoundingBoxes;

		static double deltaTime;

		static GLuint loadPNG(char* name, bool repeatX = false, bool repeatY = false);
		static void displayBoundingBox(float x1, float y1, float x2, float y2);

		static void changeToMenuScreen();
		static void changeToWorldScreen();
		static void changeToLoadingScreen();
		static void resetScreenState();
		static void changeToGameOverScreen(int score);
		static void fadeIn();
		static void fadeOut();

		static bool leftPressed;
		static bool escapePressed;
		static float mouseX;
		static float mouseY;

		static bool isOnMenu;
		static bool isPaused;
		static bool worldIsInPlay;
		static bool isLoadingScreen;
		static bool isGameOverScreen;

		static bool hasLoaded;
		static bool playButtonPressed;
		static bool quitToMenuPressed;

		static float fadeTransparency;
		static bool isFadingIn;
		static bool isFadingOut;
		static float fadeSpeed;

		static font_data bigGameFont;
		static font_data gameFont;
		static float timeSinceResize;
		static int bigFontSize;
		static int fontSize;

		static float screenWidth;
		static float screenHeight;
		static int screenWidthPixels;
		static int screenHeightPixels;

		static int highscore;

		typedef struct Point {
			float x;
			float y;

			Point operator-(const Point& p)
			{
				return Point{ x - p.x, y - p.y };
			}

			Point operator+(const Point& p)
			{
				return Point{ x + p.x, y + p.y };
			}

			float abs()
			{
				return sqrt(pow(x, 2) + pow(y, 2));
			}

			Point operator/(float d)
			{
				return Point{ x / d, y / d };
			}

			Point operator*(float d)
			{
				return Point{ x * d, y * d };
			}

			float dot(const Point& p)
			{
				return (x*p.x + y * p.y);
			}

			float distance(const Point& p)
			{
				return ((Point{ x, y } - p).abs());
			}
		};
};


