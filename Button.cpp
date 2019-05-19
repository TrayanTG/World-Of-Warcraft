#include "Button.h"
#include "Graphics.h"

Button::Button(const string &text, const COORD &topLeft): text(text), topLeft(topLeft)
{
	
}

const COORD &Button::getTopLeft()const
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

bool Button::setXY(const COORD &topLeft)
{
	if (topLeft.X < -1 || topLeft.Y < -1 || topLeft.X + text.size() + 3 >= DEF_CONSOLE_WIDTH || topLeft.Y + 4 >= DEF_CONSOLE_HEIGHT) return false;
	this->topLeft = topLeft;
	return true;
}

void Button::showButton()
{
	Graphics::getInstance().drawButton(*this);
}

bool Button::isWithin(const COORD &pos)const
{
	return (pos.X >= topLeft.X && pos.Y >= topLeft.Y && pos.X <= topLeft.X + text.size() + 3 && pos.Y <= topLeft.Y + 4);
}
