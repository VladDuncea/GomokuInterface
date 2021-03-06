/*Duncea Vlad Alexandru
Grupa 144
Fisier partial copiat de la http://lazyfoo.net/tutorials/SDL/16_true_type_fonts/index.php
*/


#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "GameWindow.h"
#include"Viewport.h"
//Texture wrapper class
class Texture
{
	//The actual hardware texture
	SDL_Texture* privTexture;

	const GameWindow & privGameWindow;

	//Image dimensions
	int privWidth;
	int privHeight;

public:
	//Initializes variables
	Texture(const GameWindow &gw);

	//Deallocates memory
	~Texture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Creates image from font string
	bool loadFromRenderedText(std::string textureText,TTF_Font *gFont,const SDL_Color textColor = { 0,0,0 });

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL,int width=0,int height=0);

	void renderCentered(int x, int y, SDL_Rect* clip = NULL, int width = 0, int height = 0);

	//Gets image dimensions
	int getWidth();
	int getHeight();
};