#include "GridSquare.h"
#include "Texture.h"

GridSquare::GridSquare(Viewport &viewp, const int buttonHeight, const int buttonWidth, const int nrStates, Texture &spriteSheet, SDL_Rect *spriteClips) : gameViewport(viewp), BUTTON_HEIGHT(buttonHeight), BUTTON_WIDTH(buttonWidth), TOTAL_STATES(nrStates), gridsprite(spriteSheet), spriteClips(spriteClips)
{
	privLocked = false;
	privPosition.x = 0;
	privPosition.y = 0;
	privCurrentSprite = 0;
}

void GridSquare::setPosition(int x, int y)
{
	privPosition.x = x;
	privPosition.y = y;
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

void GridSquare::render(int width, int height)
{
	//Show current button sprite
	gridsprite.render(privPosition.x, privPosition.y, &(spriteClips[privCurrentSprite]), width, height);
}
