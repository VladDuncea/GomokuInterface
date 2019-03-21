#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "GameWindow.h"
#include "Texture.h"

//The mouse button
class GridSquare
{

	//Button constants
	const int BUTTON_WIDTH;
	const int BUTTON_HEIGHT;
	const int TOTAL_STATES;

	//Top left position
	SDL_Point privPosition;

	//Currently used sprite
	int privCurrentSprite;

	//Reference to game window
	GameWindow &game_window;

	//Reference to sprite texture
	Texture & gridsprite;

	//Reference to sprite rect
	SDL_Rect *spriteClips;

public:
	//Initializes internal variables
	GridSquare(GameWindow& gw,const int buttonHeight,const int buttonWidth,const int nrStates,Texture &spriteSheet, SDL_Rect *spriteClips);

	//Sets top left position
	void setPosition(int x, int y);

	int width();
	int height();

	//Modify shown sprite
	void setSprite(int n);

	//Shows button sprite
	void render(int width,int height);


};
