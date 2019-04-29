/*Duncea Vlad Alexandru
Grupa 144
*/
#pragma once
#include "GameWindow.h"
class Viewport
{
	GameWindow &privGameWindow;
	SDL_Rect privRect;

public:
	Viewport(GameWindow &gw,int x,int y,int w,int h);
	Viewport(GameWindow &gw,SDL_Rect &rect);
	~Viewport();

	int height() const;
	int width() const;
	SDL_Rect& viewportRect();
	GameWindow& gameWindow() const;
};

