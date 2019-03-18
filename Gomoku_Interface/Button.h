#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "GameWindow.h"
//The mouse button
class Button
{
	//Button constants
	const int BUTTON_WIDTH;
	const int BUTTON_HEIGHT;
	const int TOTAL_BUTTONS=4;

	//Top left position
	SDL_Point mPosition;

	//Currently used global sprite
	ButtonSprite mCurrentSprite;

	//Reference to game window
	GameWindow &game_window;

public:
	//Initializes internal variables
	Button(GameWindow& gw);

	//Sets top left position
	void setPosition(int x, int y);

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	//Shows button sprite
	void render();


};
