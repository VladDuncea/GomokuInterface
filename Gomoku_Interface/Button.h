#pragma once
#include <SDL.h>
#include "Viewport.h"
#include "Renderable.h"
#include "Texture.h"
#include "TextBox.h"

class Button : public Renderable
{
	//Current viewport
	const Viewport& privViewport;
	//Button sizes
	int privHeight, privWidth;

	//Button background spritesheet
	Texture &privBackground;
	SDL_Rect* privSpriteClips;

	//Button text
	TextBox privText;

public:
	Button(Viewport & viewport,int height,int width, Texture& spriteSheet,SDL_Rect* spriteClips, TTF_Font* font,const char text[] = " ");
	~Button();

	void handleMouse(SDL_Event &e);
};

