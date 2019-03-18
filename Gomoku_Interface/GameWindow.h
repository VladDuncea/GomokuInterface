#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <exception>
#include "Texture.h"
#include "Button.h"

class GameWindow
{
	//The window we'll be rendering to
	SDL_Window* privWindow;

	//The window renderer
	SDL_Renderer* privRenderer;

	const int SCREEN_WIDTH, SCREEN_HEIGHT;

	GameWindow(GameWindow &gw):SCREEN_HEIGHT(0),SCREEN_WIDTH(0) {};

public:
	GameWindow(int screen_width,int screen_height);
	~GameWindow();

	int width()  const;
	int height() const;
	SDL_Renderer * renderer() const;
	SDL_Window * window() const;
};

