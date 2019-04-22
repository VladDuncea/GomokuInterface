#include "Renderable.h"


Renderable::Renderable()
{
	protecRendX = protecRendY = 0;
	protecRendCentered = false;
}

Renderable::~Renderable()
{
}

void Renderable::setRenderPos(int x, int y)
{
	protecRendX = x;
	protecRendY = y;
}

void Renderable::setRendCentered(bool b)
{
	protecRendCentered = b;
}
