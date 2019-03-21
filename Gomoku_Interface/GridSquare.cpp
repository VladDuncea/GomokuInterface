#include "GridSquare.h"


GridSquare::GridSquare(GameWindow &gw, const int buttonHeight, const int buttonWidth,const int nrStates) : game_window(gw), BUTTON_HEIGHT(buttonHeight), BUTTON_WIDTH(buttonWidth),TOTAL_STATES(nrStates)
{
	privPosition.x=0;
	privPosition.y = 0;
	privCurrentSprite = 0;
}

void GridSquare::setPosition(int x, int y)
{
	privPosition.x = x;
	privPosition.y = y;
}

void GridSquare::handleEvent(SDL_Event* e)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < privPosition.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > privPosition.x + BUTTON_WIDTH)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < privPosition.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > privPosition.y + BUTTON_HEIGHT)
		{
			inside = false;
		}

		//Mouse is outside button
		if (!inside)
		{
			privCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;

			case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;

			case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
			}
		}
	}
}

void GridSquare::render()
{
	//Show current button sprite
	//gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, &gSpriteClips[mCurrentSprite]);
}
