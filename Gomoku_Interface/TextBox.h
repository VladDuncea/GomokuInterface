#pragma once
#include <SDL_ttf.h>
#include "Texture.h"
#include "GameWindow.h"
#include "Renderable.h"

class TextBox: public Renderable
{
	TTF_Font * privFont;
	SDL_Color privColor;
	Texture privTextTexture;

public:
	TextBox(const GameWindow& gw, const char* c, TTF_Font* font, SDL_Color color = { 0,0,0 });
	~TextBox();
	int getHeight();
	int getWidth();
	void render();

	void chageText(const char* newText);
};

