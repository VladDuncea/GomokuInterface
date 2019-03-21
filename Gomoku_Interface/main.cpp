#include <iostream>
#include "GameWindow.h"
#include <vector>

#define GRID_WIDTH  4
#define GRID_HEIGHT  4
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
	GameWindow gw(640, 480);

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
	SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];


	for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
	{
		gSpriteClips[i].x = 0;
		gSpriteClips[i].y = i * 200;
		gSpriteClips[i].w = 200;
		gSpriteClips[i].h = 200;
	}

	//Set buttons in corners
	gButtons[0].setPosition(0, 0);
	gButtons[1].setPosition(gw.width() - BUTTON_WIDTH, 0);
	gButtons[2].setPosition(0, gw.height() - BUTTON_HEIGHT);
	gButtons[3].setPosition(gw.width() - BUTTON_WIDTH, gw.height() - BUTTON_HEIGHT);

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

			//Handle button events
			for (int i = 0; i < TOTAL_BUTTONS; ++i)
			{
				gButtons[i].handleEvent(&e);
			}
		}

		//Clear screen
		gw.clear();

		//Render buttons
		for (int i = 0; i < TOTAL_BUTTONS; ++i)
		{
			gButtons[i].render();
		}

		//Update screen
		gw.update();
	}

	return 0;
}