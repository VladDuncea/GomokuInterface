#pragma once
#include <SDL_ttf.h>
#include "Texture.h"
#include "GameWindow.h"
#include "Renderable.h"

class TextBox: public Renderable
{
	TTF_Font * privFont;
	Texture privTextTexture;

public:
	TextBox(const GameWindow &gw, const char* c,TTF_Font *font,SDL_Color color);
	~TextBox();
	int getHeight();
	void render();
};

