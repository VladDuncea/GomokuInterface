#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "GameWindow.h"
//Texture wrapper class
class Texture
{

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int privWidth;
	int privHeight;
	GameWindow & gameWindow;


public:
	//Initializes variables
	Texture(GameWindow &gw);

	//Deallocates memory
	~Texture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

#ifdef _SDL_TTF_H
	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
#endif

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

	//Gets image dimensions
	int getWidth();
	int getHeight();
};