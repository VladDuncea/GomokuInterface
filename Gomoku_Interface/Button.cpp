#include "Button.h"

Button::Button(Viewport& viewport, int height, int width, Texture& spriteSheet, SDL_Rect* spriteClips, TTF_Font* font, const char text[]) : privViewport(viewport),privBackground(spriteSheet),privText(viewport,text,font)
{
	privSpriteClips = spriteClips;
	privHeight = height;
	privWidth = width;
}

Button::~Button()
{
}

void Button::handleMouse(SDL_Event& e)
{
	//if(e.motion.x)
}
