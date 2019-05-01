#include "app.h"
#include "staticBlock.h"
#include "world.h"
#include "gem.h"
#include "button.h"

int screenWidthPixels=853, screenHeightPixels=480; //Window size in pixels
float screenWidth = 100.0, screenHeight = 100.0; //Game uses these coordinates, on a square window coordinates will go from 0 to 100 on each axis
float aspectRatio = 1.0;
float minAspectRatio = 1.5;
float maxAspectRatio = 2.5;
int minScreenWidth = 853;
int minScreenHeight = 480;
float scale = 2;
float timeScale = 1;
HWND hWnd = NULL;
bool done = false;								// Bool Variable To Exit Loop

double maxFrameTime = 0.01;	//Unusual object movement can occur if a frame takes too long to render so a max should be set
int maxFps = 200;
float UiAspectRatio = 1920.0 / 1080.0; // = 1.78
steady_clock::time_point totalFrameTime = steady_clock::now();
World world;

Button playButton = Button();
Button resumeButton = Button();
Button quitToMenuButton = Button();
Button quitToDesktopButton = Button();

GLuint loadingTexture;
GLuint menuTexture;
GLuint winTexture;
GLuint loseTexture;


//OPENGL FUNCTION PROTOTYPES
void display();				//called in winmain to draw everything to the screen
void reshape(int width, int height);				//called when the window is resized
void init();				//called in winmain when the program starts. 
void update();				//called in winmain to update variables
void displayWorld();
void displayMenu();
void displayLoading();
void displayGameOver();
void initWorld();
void pressPlay();
void quitToMenu();
void quitToDesktop();
void resume();

/*************    START OF OPENGL FUNCTIONS   ****************/
void display()									
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	if (App::isOnMenu)
	{
		displayMenu();
	}
	else if (App::isLoadingScreen)
	{
		displayLoading();
	}
	else if (App::worldIsInPlay)
	{
		displayWorld();
	}
	else if (App::isGameOverScreen)
	{
		displayGameOver();
	}

	glColor4f(0.0, 0.0, 0.0, 1.0 - App::fadeTransparency);
	glBegin(GL_POLYGON);
	glVertex2f(0, screenHeight);
	glVertex2f(screenWidth, screenHeight);
	glVertex2f(screenWidth, 0);
	glVertex2f(0, 0);
	glEnd();
	glColor4f(1.0, 1.0, 1.0, 1.0);
	
	glFlush();
}

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidthPixels=width; screenHeightPixels = height;           // to ensure the mouse coordinates match 
																	// we will use these values to set the coordinate system
	if ((float)width / (float)height > 1.0)
	{
		screenHeight = 100.0 * scale;
		screenWidth = 100.0 * (float)width * scale / (float)height;
	}
	else if ((float)height / (float)width > 1)
	{
		screenWidth = 100.0 * scale;
		screenHeight = 100.0 * scale * (float)height / (float)width;
	}
	else
	{
		screenHeight = 100.0 * scale;
		screenWidth = 100.0 * scale;
	}

	aspectRatio = screenWidth / screenHeight;


	App::gameFont.init("gameFont.TTF", screenWidthPixels * App::fontSize / screenWidth);
	App::bigGameFont.init("gameFont.TTF", screenWidthPixels * App::bigFontSize / screenWidth);
	App::timeSinceResize = 0.0;
	
	glViewport(0,0,width, height);						// Reset the current viewport

	glMatrixMode(GL_PROJECTION);						// select the projection matrix stack
	glLoadIdentity();									// reset the top of the projection matrix to an identity matrix

	gluOrtho2D(0,screenWidth,0,screenHeight);           // set the coordinate system for the window

	glMatrixMode(GL_MODELVIEW);							// Select the modelview matrix stack
	glLoadIdentity();									// Reset the top of the modelview matrix to an identity matrix

	App::screenWidth = screenWidth;
	App::screenHeight = screenHeight;
	App::screenWidthPixels = width;
	App::screenHeightPixels = height;
}
void init()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	loadingTexture = App::loadPNG("menu/loading.png");
	menuTexture = App::loadPNG("menu/title.png");
	winTexture = App::loadPNG("menu/win.png");
	loseTexture = App::loadPNG("menu/lose.png");

	App::gameFont.init("gameFont.TTF", screenWidthPixels * App::fontSize / screenWidth);
	App::bigGameFont.init("gameFont.TTF", screenWidthPixels * App::bigFontSize / screenWidth);
	
	playButton.initialise(0.5, 0.5, 300, 87, 0.2, "PLAY", &pressPlay);
	resumeButton.initialise(0.5, 0.55, 300, 87, 0.2, "RESUME", &resume);
	quitToMenuButton.initialise(0.5, 0.4, 300, 87, 0.2, "QUIT TO MENU", &quitToMenu);
	quitToDesktopButton.initialise(0.5, 0.25, 300, 87, 0.2, "QUIT TO DESKTOP", &quitToDesktop);
	
	App::changeToMenuScreen();
	
	glClearColor(0.0, 0.0, 0.0, 0.0);						
}

void initWorld()
{
	world.reset();
	world.init();
	App::hasLoaded = true;
	App::isFadingOut = true;
}

void update()
{
	App::timeSinceResize += App::deltaTime;
	//Check play button if menu is showing and if not already fading out
	//If faded out then change to loading screen
	if (App::isOnMenu)
	{
		if (!App::playButtonPressed)
		{
			playButton.checkIfButtonHighlighted(screenWidth, screenHeight);
		}
		else if (App::fadeTransparency == 0.0)
		{
			App::changeToLoadingScreen();
		}
	}
	//If loading screen has faded in then start to load world
	//Program freezes for a few seconds while world initialises
	//fade out load screen after world load
	else if (App::isLoadingScreen)
	{
		if (!App::isFadingIn && !App::hasLoaded)
		{
			initWorld();
		}
		else if (!App::isFadingOut && App::hasLoaded)
		{
			App::changeToWorldScreen();
		}
	}
	else if (App::worldIsInPlay)
	{
		if (!App::quitToMenuPressed)
		{		
			if (App::escapePressed)
			{
				if (App::isPaused)
				{
					App::isPaused = false;
				}
				else
				{
					App::isPaused = true;
				}
				
			}
			if (world.gameEnded)
			{
				App::changeToGameOverScreen(world.score);
			}
			else if (App::isPaused)
			{
				resumeButton.checkIfButtonHighlighted(screenWidth, screenHeight);
				quitToMenuButton.checkIfButtonHighlighted(screenWidth, screenHeight);
				quitToDesktopButton.checkIfButtonHighlighted(screenWidth, screenHeight);
			}
			else
			{
				world.update();
			}
		}
		else if (App::fadeTransparency == 0.0)
		{
			App::changeToMenuScreen();
		}
	}
	else if (App::isGameOverScreen)
	{
		quitToMenuButton.checkIfButtonHighlighted(screenWidth, screenHeight);
		quitToDesktopButton.checkIfButtonHighlighted(screenWidth, screenHeight);

		if (App::quitToMenuPressed && !App::isFadingOut)
		{
			App::changeToMenuScreen();
		}
	}

	App::escapePressed = false;
	App::leftPressed = false;
	App::fadeOut();
	App::fadeIn();
}

void displayMenu()
{
	float w;
	float h;
	if (aspectRatio > UiAspectRatio)
	{
		w = 0;
		h = (1.0 - (UiAspectRatio / aspectRatio)) / 2.0;
	}
	else
	{
		h = 0;
		w = (1.0-(aspectRatio / UiAspectRatio)) / 2.0;
	}

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, menuTexture);
	glBegin(GL_POLYGON);
	glTexCoord2f(w, 1-h); glVertex2f(0, screenHeight);
	glTexCoord2f(1-w, 1-h); glVertex2f(screenWidth, screenHeight);
	glTexCoord2f(1-w, h); glVertex2f(screenWidth, 0);
	glTexCoord2f(w, h); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	playButton.display(screenWidth, screenHeight);
}

void displayLoading()
{
	float w;
	float h;
	if (aspectRatio > UiAspectRatio)
	{
		w = 0;
		h = (1.0 - (UiAspectRatio / aspectRatio)) / 2.0;
	}
	else
	{
		h = 0;
		w = (1.0 - (aspectRatio / UiAspectRatio)) / 2.0;
	}

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, loadingTexture);
	glBegin(GL_POLYGON);
	glTexCoord2f(w, 1-h); glVertex2f(0, screenHeight);
	glTexCoord2f(1-w, 1-h); glVertex2f(screenWidth, screenHeight);
	glTexCoord2f(1-w, h); glVertex2f(screenWidth, 0);
	glTexCoord2f(w, h); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void displayGameOver()
{	
	float s = 0.2;
	float w = 786 * s;
	float h = 134 * s;
	GLuint t = loseTexture;
	if (world.won)
	{
		t = winTexture;
		w = 737 * s;
		h = 100 * s;
	}

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, t);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(screenWidth / 2 - w / 2, screenHeight - h*1 - 5);
	glTexCoord2f(1, 1); glVertex2f(screenWidth / 2 + w / 2, screenHeight - h*1 - 5);
	glTexCoord2f(1, 0); glVertex2f(screenWidth / 2 + w / 2, screenHeight - h*2 - 5);
	glTexCoord2f(0, 0); glVertex2f(screenWidth / 2 - w / 2, screenHeight - h*2 - 5);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	int charWidth = App::screenWidthPixels * App::fontSize / screenWidth;
	int noOfChars = 18;
	float paddingX = (-noOfChars * charWidth) / 2;

	glTranslatef((App::screenWidthPixels / App::screenWidth) * (screenWidth / 2) + paddingX, (App::screenHeightPixels / App::screenHeight) * (screenHeight * 0.63), 0.0);
	print(App::bigGameFont, 0, 0, "SCORE: %05d", world.score);
	glPopMatrix();
	glPushMatrix();
	glTranslatef((App::screenWidthPixels / App::screenWidth) * (screenWidth / 2) + paddingX*1.3, (App::screenHeightPixels / App::screenHeight) * (screenHeight * 0.53), 0.0);
	print(App::bigGameFont, 0, 0, "HIGHSCORE: %05d", App::highscore);
	glPopMatrix();
	
	quitToMenuButton.display(screenWidth, screenHeight);
	quitToDesktopButton.display(screenWidth, screenHeight);
}

void displayWorld()
{
	//Render world and bounding boxes
	glPushMatrix();
	world.moveCamera(screenWidth, screenHeight);
	world.displayBackground(screenWidth, screenHeight);
	world.display();

	if (App::displayBoundingBox)
	{
		world.displayPlayerCameraBox(screenWidth, screenHeight);
		world.displayWorldBoundaries();
	}
	glPopMatrix();

	//Display gem UI
	for (Gem gem : world.gems)
	{
		gem.displayUI(screenWidth, screenHeight);
	}

	//Display player hearts
	int pHealth = world.player->health;
	int heartCount = 0;

	glPushMatrix();
	int charWidth = App::screenWidthPixels * App::fontSize / screenWidth;
	int noOfChars = 18;
	float paddingX = (-noOfChars * charWidth) / 2;

	glTranslatef((App::screenWidthPixels / App::screenWidth) * (screenWidth / 2) + paddingX, (App::screenHeightPixels / App::screenHeight) * (screenHeight * 0.93), 0.0);
	print(App::bigGameFont, 0, 0, "SCORE: %05d", world.score);
	glPopMatrix();

	while (pHealth > 0)
	{
		int heartsToRemove = pHealth % 4;
		world.player->displayHearts(heartCount, heartsToRemove, screenWidth, screenHeight);
		heartCount++;
		//If hearts to remove is 0 it means a whole heart was displayed (4%4 = 0)
		if (heartsToRemove == 0)
		{
			pHealth -= 4;
		}
		else
		{
			pHealth -= heartsToRemove;
		}
	}

	if (App::isPaused)
	{
		glColor4f(0.0, 0.0, 0.0, 0.5);
		glBegin(GL_POLYGON);
		glVertex2f(0, screenHeight);
		glVertex2f(screenWidth, screenHeight);
		glVertex2f(screenWidth, 0);
		glVertex2f(0, 0);
		glEnd();
		glColor4f(1.0, 1.0, 1.0, 1.0);

		resumeButton.display(screenWidth, screenHeight);
		quitToMenuButton.display(screenWidth, screenHeight);
		quitToDesktopButton.display(screenWidth, screenHeight);
	}
}

void pressPlay()
{
	App::isFadingOut = true;
	App::isFadingIn = false;
	App::playButtonPressed = true;
}

void quitToMenu()
{
	App::quitToMenuPressed = true;
	App::isFadingOut = true;
}

void quitToDesktop()
{
	done = true;
}

void resume()
{
	App::isPaused = false;
}
/**************** END OPENGL FUNCTIONS *************************/

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height); //creates the window
int WINAPI WinMain(	HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function

//win32 global variabless
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure


	AllocConsole();
	FILE *stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);
	
	// Create Our OpenGL Window
	if (!CreateGLWindow("OpenGL Win32 Example",screenWidthPixels,screenHeightPixels))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			update();					// update variables
			display();					// Draw The Scene
			SwapBuffers(hDC);				// Swap Buffers (Double Buffering)

			App::deltaTime = double(duration_cast<duration<double>>(steady_clock::now() - totalFrameTime).count()) * timeScale;

			//Limit fps to prevent cpu being hogged for unneccesarily large fps
			if (timeScale / App::deltaTime > maxFps)
			{
				double timeToWait = (1.0 / maxFps) - App::deltaTime * 1.0/timeScale;
				while (timeToWait > 0.0)
				{
					std::this_thread::sleep_for(std::chrono::nanoseconds{ 0 });
					App::deltaTime = double(duration_cast<duration<double>>(steady_clock::now() - totalFrameTime).count());
					timeToWait = (1.0 / maxFps) - App::deltaTime * 1.0 / timeScale;
				}
			}

			App::deltaTime = double(duration_cast<duration<double>>(steady_clock::now() - totalFrameTime).count()) * timeScale;
			totalFrameTime = steady_clock::now();

			//Force delta time to be less than max frame time
			if (App::deltaTime > maxFrameTime)
			{
				App::deltaTime = maxFrameTime;
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}
		break;

		case WM_SIZE:								// Resize The OpenGL Window
		{
			reshape(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
		break;

		case WM_LBUTTONDOWN:
			{
	            App::mouseX = LOWORD(lParam) * screenWidth / screenWidthPixels;          
				App::mouseY = (screenHeightPixels - HIWORD(lParam)) * screenHeight / screenHeightPixels;
				App::leftPressed = true;
			}
		break;

		case WM_LBUTTONUP:
			{
			}
		break;

		case WM_MOUSEMOVE:
			{
				App::mouseX = LOWORD(lParam) * screenWidth / screenWidthPixels;
				App::mouseY = (screenHeightPixels - HIWORD(lParam)) * screenHeight / screenHeightPixels;
			}
		break;
		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			App::keys[wParam] = true;					// If So, Mark It As TRUE
			if (wParam == VK_CONTROL)
			{
				if (App::shouldDrawBoundingBoxes)
				{
					App::shouldDrawBoundingBoxes = false;
				}
				else
				{
					App::shouldDrawBoundingBoxes = true;
				}
			}
			else if (wParam == VK_ESCAPE)
			{
				if (App::escapePressed)
				{
					App::escapePressed = false;
				}
				else
				{
					App::escapePressed = true;
				}
			}
			return 0;								// Jump Back
		}
		break;
		case WM_KEYUP:								// Has A Key Been Released?
		{
			App::keys[wParam] = false;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}
		break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*/
 
bool CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}
	
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	reshape(width, height);					// Set Up Our Perspective GL Screen

	init();
	
	return true;									// Success
}



