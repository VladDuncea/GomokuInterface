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
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

//Using SDL, SDL_image, standard IO, and string
//Buttons objects
//LButton gButtons[TOTAL_BUTTONS];

/*
bool loadAssets()
{
	//Loading success flag
	bool success = true;

	//Load sprites
		//Set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			gSpriteClips[i].x = 0;
			gSpriteClips[i].y = i * 200;
			gSpriteClips[i].w = BUTTON_WIDTH;
			gSpriteClips[i].h = BUTTON_HEIGHT;
		}

		//Set buttons in corners
		gButtons[0].setPosition(0, 0);
		gButtons[1].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, 0);
		gButtons[2].setPosition(0, SCREEN_HEIGHT - BUTTON_HEIGHT);
		gButtons[3].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);
	}

	if (!success)
		throw - 1;
}*/



int main(int argc, char* args[])
{
	
	//Start up SDL and create game window
	GameWindow gw(950, 950);
	
	//Load assets

	//Button sprites
	
	Texture ButtonSpriteSheet(gw);
	try
	{
		ButtonSpriteSheet.loadFromFile("button.png");
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
		spriteClips[i].y = i * 200;
		spriteClips[i].w = 300;
		spriteClips[i].h = 200;
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
					gridSquares[i].setSprite(0);
				}
				if(buttonNr<=360)
					gridSquares[buttonNr].setSprite(1);

				//Render the grid
				for (int i = 0; i < GRID_HEIGHT*GRID_WIDTH; i++)
				{
					gridSquares[i].render(gw.width()/GRID_WIDTH,gw.height()/GRID_HEIGHT);
				}
				/*
				//Mouse is inside button
				else
				{
					//Set mouse over sprite
					switch (e.type)
					{
					case SDL_MOUSEMOTION:
						mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
						break;

					case SDL_MOUSEBUTTONDOWN:
						mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
						break;

					case SDL_MOUSEBUTTONUP:
						mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
						break;
					}
				}*/

				gw.update();
			}

		}
	}
	
	return 0;
}