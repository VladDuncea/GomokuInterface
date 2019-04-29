#include "Button.h"

Button::Button(Viewport& viewport, int height, int width, Texture* spriteSheet, SDL_Rect* spriteClips, TTF_Font* font, const char text[]) : privViewport(viewport),privBackground(spriteSheet),privText(viewport,text,font)
{
	//Set text 
	privText.setRendCentered(true);
	for (int i = 0; i < BUTTON_TOTAL; i++)
	{
		privSpriteClips[i] = spriteClips[i];
	}
	
	privHeight = height;
	privWidth = width;
	privState = BUTTON_IDLE;
	privIsToggle = false;
}

Button::~Button()
{
}

void Button::handleMouse(SDL_Event& e)
{
	//Get mouse coords in current viewport
	int mouseX = e.motion.x - privViewport.viewportRect().x;
	int mouseY = e.motion.y - privViewport.viewportRect().y;
	//Check if inside button
	int checkX = mouseX - protecRendPos.x;
	int checkY = mouseY - protecRendPos.y;
	if (protecRendCentered)
	{
		checkX += privWidth/2;
		checkY += privHeight/2;
	}
	if (checkX >= 0 && checkX <= privWidth &&checkY>=0 && checkY<=privHeight)
	{
		//Mouse is over the button
		if (e.type == SDL_MOUSEMOTION)
		{
			//Go to hover if button is not pressed or not in toggle mode
			if (!privIsToggle || privState != BUTTON_CLICK)
				privState = BUTTON_HOVER;
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			if(!privIsToggle)
				privState = BUTTON_CLICK;
			else
			{
				privState = privState == BUTTON_CLICK ? BUTTON_HOVER : BUTTON_CLICK;
			}
			//TODO handle click
		}
		else if (e.type == SDL_MOUSEBUTTONUP)
		{
			//On mouse up go back to hover if button is in click mode
			if(!privIsToggle)
				privState = BUTTON_HOVER;
		}
	}
	else
	{
		//Change the state of the button only if it is not pressed or if it is in click mode
		if(!privIsToggle||privState == BUTTON_HOVER)
			privState = BUTTON_IDLE;
	}
}

bool Button::isClicked()
{
	return privState == BUTTON_CLICK ? true : false;
}

void Button::unToggle()
{
	if (privIsToggle)
		privState = BUTTON_IDLE;
}

void Button::setToggle(bool value)
{
	privIsToggle = value;
}

void Button::render()
{
	if (!protecRendEnabled)
		return;
	//Render centered
	
	if (protecRendCentered)
	{
		privText.setRenderPos(protecRendPos.x, protecRendPos.y);
		privBackground->renderCentered(protecRendPos.x, protecRendPos.y, &(privSpriteClips[privState]), privWidth, privHeight);
	}
	else
	{
		privText.setRenderPos(protecRendPos.x + privWidth / 2, protecRendPos.y + privHeight / 2);
		privBackground->render(protecRendPos.x, protecRendPos.y, &(privSpriteClips[privState]), privWidth, privHeight);
	}
	privText.render();
}
