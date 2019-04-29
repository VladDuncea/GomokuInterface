/*Duncea Vlad Alexandru
Grupa 144
*/


#pragma once
#include <SDL_ttf.h>
#include "Texture.h"
#include "Viewport.h"
#include "Renderable.h"

class TextBox: public Renderable
{
	TTF_Font * privFont;
	SDL_Color privColor;
	Texture privTextTexture;

public:
	TextBox(const Viewport& viewport, const char* c, TTF_Font* font, SDL_Color color = { 0,0,0 });
	~TextBox();
	int getHeight();
	int getWidth();
	void render();

	void chageText(const char* newText);
};

