#include "Renderable.h"


Renderable::Renderable()
{
	protecRendPos.x = protecRendPos.y = 0;
	protecRendCentered = false;
	protecRendEnabled = true;
}

Renderable::~Renderable()
{
}

void Renderable::setRenderPos(int x, int y)
{
	protecRendPos.x = x;
	protecRendPos.y = y;
}

POINT Renderable::getRenderPos()
{
	return protecRendPos;
}

void Renderable::setRendCentered(bool b)
{
	protecRendCentered = b;
}

void Renderable::setRendEnabled(bool b)
{
	protecRendEnabled = b;
}
