/*Duncea Vlad Alexandru
Grupa 144
Fisier partial copiat de la http://lazyfoo.net/tutorials/SDL/16_true_type_fonts/index.php
*/

#include "Texture.h"

Texture::Texture(const GameWindow &gw) : privGameWindow(gw)
{
	//Initialize
	privTexture = NULL;
	privWidth = 0;
	privHeight = 0;
}

Texture::~Texture()
{
	//Deallocate
	free();
}

bool Texture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		//SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Scale image to dimensions
		
		//SDL_BlitScaled(gStretchedSurface, NULL, gScreenSurface, &stretchRect);

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(privGameWindow.renderer(), loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			privWidth = loadedSurface->w;
			privHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	privTexture = newTexture;
	return privTexture != NULL;
}

bool Texture::loadFromRenderedText(std::string textureText,TTF_Font *font, const SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		privTexture = SDL_CreateTextureFromSurface(privGameWindow.renderer(), textSurface);
		if (privTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			privWidth = textSurface->w;
			privHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return privTexture != NULL;
}

void Texture::free()
{
	//Free texture if it exists
	if (privTexture != NULL)
	{
		SDL_DestroyTexture(privTexture);
		privTexture = NULL;
		privWidth = 0;
		privHeight = 0;
	}
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(privTexture, red, green, blue);
}

void Texture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(privTexture, blending);
}

void Texture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(privTexture, alpha);
}

void Texture::render(int x, int y, SDL_Rect* clip,int width,int height)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, privWidth, privHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = width;
		renderQuad.h = height;
	}

	//Render to screen
	SDL_RenderCopyEx(privGameWindow .renderer(), privTexture, clip, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
}

void Texture::renderCentered(int x, int y, SDL_Rect* clip, int width, int height)
{
	//Normal render but with coordonates as center of image
	if(clip == NULL)
		render(x - privWidth / 2, y - privHeight / 2,clip,width,height);
	else
		render(x - width / 2, y - height / 2, clip, width, height);
}

int Texture::getWidth()
{
	return privWidth;
}

int Texture::getHeight()
{
	return privHeight;
}
