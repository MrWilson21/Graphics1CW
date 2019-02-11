#pragma once

#include <windows.h>		// Header file for Windows
#include "Image_Loading/nvImage.h"
#include <iostream>
#include <gl\gl.h>			// Header file for the OpenGL32 Library
#include <gl\glu.h>			// Header file for the GLu32 Library
#include <time.h>
#include <string>
using namespace std;

class App
{
	public:
		static bool keys[256];
		static const double degToRad;

		static double deltaTime;

		static GLuint loadPNG(char* name);
};


