#include "GameWindow.h"

GameWindow::GameWindow(int screen_width,int screen_height): SCREEN_HEIGHT(screen_height),SCREEN_WIDTH(screen_width)
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		privWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
		if (privWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			privRenderer = SDL_CreateRenderer(privWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(privRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}
	if (success == false)
		throw -1;
}


GameWindow::~GameWindow()
{
	//Free loaded images
	//gButtonSpriteSheetTexture.free();

	//Destroy window	
	SDL_DestroyRenderer(privRenderer);
	SDL_DestroyWindow(privWindow);

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int GameWindow::width() const
{
	return SCREEN_WIDTH;
}

int GameWindow::height() const
{
	return SCREEN_HEIGHT;
}

SDL_Renderer * GameWindow::renderer() const
{
	return privRenderer;
}

SDL_Window * GameWindow::window() const
{
	return privWindow;
}
