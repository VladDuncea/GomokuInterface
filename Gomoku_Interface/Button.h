#pragma once
#include <SDL.h>
#include "Viewport.h"
#include "Renderable.h"
#include "Texture.h"
#include "TextBox.h"

constexpr auto BUTTON_TOTAL_STATES = 3;

class Button : public Renderable
{
	//Possible button states
	enum ButtonState
	{
		BUTTON_IDLE,
		BUTTON_HOVER,
		BUTTON_CLICK,
		BUTTON_TOTAL
	};

	ButtonState privState;

	//Current viewport
	Viewport& privViewport;
	//Button sizes
	int privHeight, privWidth;

	//Button background spritesheet
	Texture* privBackground;
	SDL_Rect privSpriteClips[BUTTON_TOTAL];

	//Button text
	TextBox privText;

	//Toggle variable
	bool privIsToggle;

public:
	Button(Viewport & viewport,int height,int width, Texture* spriteSheet,SDL_Rect* spriteClips, TTF_Font* font,const char text[] = " ");
	~Button();

	void handleMouse(SDL_Event &e);

	bool isClicked();

	void unToggle();

	void setToggle(bool value);

	void render();
};

