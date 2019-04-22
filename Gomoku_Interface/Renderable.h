#pragma once
#include "GameWindow.h"

class Renderable
{
protected:
	int protecRendX,protecRendY;
	bool protecRendCentered;
public:
	Renderable();
	virtual ~Renderable() = 0;
	virtual void render(void)=0;
	void setRenderPos(int x, int y);
	void setRendCentered(bool b);
};

