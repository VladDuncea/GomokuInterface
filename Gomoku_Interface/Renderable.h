#pragma once
#include "GameWindow.h"

class Renderable
{
protected:
	SDL_Point protecRendPos;
	bool protecRendCentered;
	bool protecRendEnabled;
public:
	Renderable();
	virtual ~Renderable() = 0;
	virtual void render(void)=0;
	void setRenderPos(int x, int y);
	SDL_Point getRenderPos();
	void setRendCentered(bool b);
	void setRendEnabled(bool b);
};

