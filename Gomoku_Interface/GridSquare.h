/*Duncea Vlad Alexandru
Grupa 144
*/


#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "Viewport.h"
#include "Texture.h"
#include "Renderable.h"

//The mouse button
class GridSquare :public Renderable
{

	//Button constants
	const int BUTTON_WIDTH;
	const int BUTTON_HEIGHT;
	const int TOTAL_STATES;

	//Lock the square once it is chosen
	bool privLocked;

	//Currently used sprite
	int privCurrentSprite;

	//Reference to game window
	Viewport &gameViewport;

	//Reference to sprite texture
	Texture & gridsprite;

	//Reference to sprite rect
	SDL_Rect *spriteClips;

public:
	//Initializes internal variables
	GridSquare(Viewport& viewp,const int squareHeight,const int squareWidth,const int nrStates,Texture &spriteSheet, SDL_Rect *spriteClips);

	int width();
	int height();

	//Modify shown sprite
	void setSprite(int n);
	//Get shown sprite id
	int getSprite();

	//Setter and getter for locked state
	void locked(bool newState);

	bool locked();

	void render();


};
