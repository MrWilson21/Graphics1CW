#pragma once

#include <windows.h>		// Header file for Windows
#include "Image_Loading/nvImage.h"
#include <iostream>
#include <gl\gl.h>			// Header file for the OpenGL32 Library
#include <gl\glu.h>			// Header file for the GLu32 Library
#include <time.h>
#include <string>
#include <vector>
using namespace std;

class App
{
	public:
		static bool keys[256];
		static const double degToRad;
		static bool shouldDrawBoundingBoxes;

		static double deltaTime;

		static GLuint loadPNG(char* name);
		static void displayBoundingBox(float x1, float y1, float x2, float y2);
};


