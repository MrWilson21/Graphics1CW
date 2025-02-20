#include "app.h"
#pragma once

class Button
{
public:
	Button();
	void initialise(float x, float y, float width, float height, float scale, string text, void(*function)());
	void checkIfButtonHighlighted(float screenWidth, float screenHeight);
	void display(float screenWidth, float screenHeight);
	
	GLuint inactiveTexture;
	GLuint activeTexture;

private:
	float x;
	float y;
	float width;
	float height;
	string text;
	void(*function)();
	bool highlighted;
};
