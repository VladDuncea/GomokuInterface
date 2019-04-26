#include "TextBox.h"


TextBox::TextBox(const Viewport &viewport, const char * c, TTF_Font * font, SDL_Color color): privTextTexture(viewport)
{
	privColor = color;
	privFont = font;
	privTextTexture.loadFromRenderedText(c,font,color);
}

TextBox::~TextBox()
{
}

int TextBox::getHeight()
{
	return privTextTexture.getHeight();
}

int TextBox::getWidth()
{
	return privTextTexture.getWidth();
}

void TextBox::render()
{
	if (!protecRendEnabled)
		return;
	//Render centered
	if (protecRendCentered)
	{
		privTextTexture.renderCentered(protecRendPos.x, protecRendPos.y);
	}
	else
	{
		privTextTexture.render(protecRendPos.x, protecRendPos.y);
	}
}

void TextBox::chageText(const char* newText)
{
	privTextTexture.loadFromRenderedText(newText, privFont, privColor);
}
