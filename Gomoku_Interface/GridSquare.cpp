/*Duncea Vlad Alexandru
Grupa 144
*/


#include "GridSquare.h"
#include "Texture.h"

GridSquare::GridSquare(Viewport &viewp, const int buttonHeight, const int buttonWidth, const int nrStates, Texture &spriteSheet, SDL_Rect *spriteClips) : gameViewport(viewp), BUTTON_HEIGHT(buttonHeight), BUTTON_WIDTH(buttonWidth), TOTAL_STATES(nrStates), gridsprite(spriteSheet), spriteClips(spriteClips)
{
	privLocked = false;
	privCurrentSprite = 0;
}

int GridSquare::width()
{
	return BUTTON_WIDTH;
}

int GridSquare::height()
{
	return BUTTON_HEIGHT;
}

void GridSquare::setSprite(int n)
{
	if (n < TOTAL_STATES && !privLocked)
		privCurrentSprite = n;
}

int GridSquare::getSprite()
{
	return privCurrentSprite;
}

void GridSquare::locked(bool newState)
{
	privLocked = newState;
}

bool GridSquare::locked()
{
	return privLocked;
}

void GridSquare::render()
{
	gridsprite.render(protecRendPos.x, protecRendPos.y, &(spriteClips[privCurrentSprite]), BUTTON_WIDTH, BUTTON_HEIGHT);
}
