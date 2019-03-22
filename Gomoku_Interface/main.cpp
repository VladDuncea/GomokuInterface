#include <iostream>
#include "GameWindow.h"
#include "Texture.h"
#include "GridSquare.h"
#include <vector>

#define GRID_WIDTH  19
#define GRID_HEIGHT  19
#define TOTAL_BUTTONS GRID_HEIGHT*GRID_WIDTH

enum ButtonSprite
{
	GRID_DEFAULT = 0,
	GRID_BLUE_PIECE= 1,
	GRID_RED_PIECE = 2,
	BUTTON_SPRITE_TOTAL = 3
};


int main(int argc, char* args[])
{
	
	//Start up SDL and create game window
	GameWindow gw(GRID_HEIGHT*30, GRID_WIDTH*30);
	
	//Load assets

	//Button sprites
	
	Texture ButtonSpriteSheet(gw);
	try
	{
		ButtonSpriteSheet.loadFromFile("piece.png");
	}
	catch (int e)
	{
		printf("Failed to load button sprite texture!\n");
		exit(-1);
	}
	//Mouse button sprites
	SDL_Rect spriteClips[BUTTON_SPRITE_TOTAL];
	
	

	//Select sprites in sprite sheet
	for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
	{
		spriteClips[i].x = 0;
		spriteClips[i].y = i * 50;
		spriteClips[i].w = 50;
		spriteClips[i].h = 50;
	}

	//All the squares in the grid
	std::vector<GridSquare> gridSquares;
	//Set buttons in corners
	GridSquare gs(gw, gw.height() / GRID_HEIGHT, gw.width() / GRID_WIDTH, BUTTON_SPRITE_TOTAL, ButtonSpriteSheet,spriteClips);
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			gs.setPosition(gs.width()*j, gs.height()*i);
			gridSquares.push_back(gs);
		}
	}

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//While application is running
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			//If mouse event happened
			if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
			{
				gw.clear();
				//Get mouse position
				int x, y;
				//Get mouse and account for out of bouds
				SDL_GetMouseState(&x, &y);
				
				printf("x: %d y:%d\n", x, y);
				//Get button coordinates
				x /= gridSquares[0].width();
				y /= gridSquares[0].height();
				int buttonNr = y * GRID_WIDTH + x;
				//Set all grid squares to empty
				for (int i = 0; i < GRID_HEIGHT*GRID_WIDTH; i++)
				{
					gridSquares[i].setSprite(GRID_DEFAULT);
				}
				if(buttonNr<=360)
					gridSquares[buttonNr].setSprite(GRID_RED_PIECE);

				//Render the grid
				for (int i = 0; i < GRID_HEIGHT*GRID_WIDTH; i++)
				{
					gridSquares[i].render(gw.width()/GRID_WIDTH,gw.height()/GRID_HEIGHT);
				}
				gw.update();
			}

		}
	}
	
	return 0;
}