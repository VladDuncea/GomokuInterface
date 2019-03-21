#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "GameWindow.h"
//The mouse button
class GridSquare
{

	//Button constants
	const int BUTTON_WIDTH;
	const int BUTTON_HEIGHT;
	const int TOTAL_BUTTONS=4;
	const int TOTAL_STATES;

	//Top left position
	SDL_Point privPosition;

	//Currently used sprite
	int privCurrentSprite;

	//Reference to game window
	GameWindow &game_window;

	//REference to sprite texture
	Texture & gridsprite;

public:
	//Initializes internal variables
	GridSquare(GameWindow& gw,const int buttonHeight,const int buttonWidth,const int nrStates);

	//Sets top left position
	void setPosition(int x, int y);

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	//Shows button sprite
	void render();


};
