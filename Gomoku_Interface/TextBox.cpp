#include "TextBox.h"


TextBox::TextBox(const GameWindow &gw, const char * c, TTF_Font * font, SDL_Color color): privTextTexture(gw)
{
	privTextTexture.loadFromRenderedText(c,font,color);
}

TextBox::~TextBox()
{
}

int TextBox::getHeight()
{
	return privTextTexture.getHeight();
}

void TextBox::render()
{
	//Render centered
	if (protecRendCentered)
	{
		privTextTexture.renderCentered(protecRendX, protecRendY);
	}
	else
	{
		privTextTexture.render(protecRendX, protecRendY);
	}
}
