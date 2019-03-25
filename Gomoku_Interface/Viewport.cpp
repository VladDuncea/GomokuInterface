#include "Viewport.h"


Viewport::Viewport(GameWindow & gw,int x, int y, int w, int h) : privGameWindow(gw)
{
	privRect.x = x;
	privRect.y = y;
	privRect.w = w;
	privRect.h = h;
}

Viewport::Viewport(GameWindow & gw, SDL_Rect & rect): privGameWindow(gw),privRect(rect)
{
}

Viewport::~Viewport()
{
}

int Viewport::height() const
{
	return privRect.h;
}

int Viewport::width() const
{
	return privRect.w;
}

SDL_Rect& Viewport::viewportRect()
{
	return privRect;
}
