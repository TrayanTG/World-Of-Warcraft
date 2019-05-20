#include "Button.h"
#include "Graphics.h"

Button::Button(const string &text, const Coord &topLeft): text(text), topLeft(topLeft)
{
	
}

const Coord &Button::getTopLeft()const
{
	return topLeft;
}

int Button::getLen()const
{
	return text.size();
}

const string &Button::getText()const
{
	return text;
}

bool Button::setXY(const Coord &topLeft)
{
	if (topLeft.X < -1 || topLeft.Y < -1 || topLeft.X + text.size() + 3 >= DEF_CONSOLE_WIDTH || topLeft.Y + 4 >= DEF_CONSOLE_HEIGHT) return false;
	this->topLeft = topLeft;
	return true;
}

void Button::showButton()
{
	Graphics::drawButton(*this);
}

bool Button::isWithin(const Coord &pos)const
{
	return (pos.X >= topLeft.X && pos.Y >= topLeft.Y && pos.X <= topLeft.X + (int)text.size() + 3 && pos.Y <= topLeft.Y + 4);
}
