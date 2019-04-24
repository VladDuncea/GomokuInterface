#pragma once
#include "GameWindow.h"
#include <Windows.h>

class Renderable
{
protected:
	POINT protecRendPos;
	bool protecRendCentered;
	bool protecRendEnabled;
public:
	Renderable();
	virtual ~Renderable() = 0;
	virtual void render(void)=0;
	void setRenderPos(int x, int y);
	POINT getRenderPos();
	void setRendCentered(bool b);
	void setRendEnabled(bool b);
};

