#include "Button.h"
#include "Graphics.h"

Button::Button(const char *text, int tlx, int tly)
{
	this->text = new char[strlen(text) + 1];
	strcpy(this->text, text);
	lenght = strlen(text);
	this->tlx = tlx;
	this->tly = tly;
}

Button::Button(const Button &other)
{
	text = new char[strlen(other.text) + 1];
	strcpy(text, other.text);
	lenght = other.lenght;
	tlx = other.tlx;
	tly = other.tly;
}

Button& Button::operator= (const Button &other)
{
	if (this == &other)return *this;
	text = new char[strlen(other.text) + 1];
	strcpy(text, other.text);
	lenght = other.lenght;
	tlx = other.tlx;
	tly = other.tly;
	return *this;
}

Button::~Button()
{
	delete[] text;
}

int Button::getTLX()const
{
	return tlx;
}

int Button::getTLY()const
{
	return tly;
}

int Button::getLen()const
{
	return lenght;
}

const char *Button::getText()const
{
	return text;
}


bool Button::setXY(int tlx, int tly)
{
	if (tlx < 0 || tly < 0 || tlx + lenght + 3 >= DEF_CONSOLE_WIDTH || tly + 4 >= DEF_CONSOLE_HEIGHT) return false;
	this->tlx = tlx;
	this->tly = tly;
	return true;
}

bool Button::showButton()
{
	return Graphics::getInstance().drawButton(*this);
}

bool Button::isWithin(int x, int y)const
{
	return (x >= tlx && y >= tly && x <= tlx + lenght + 3 && y <= tly + 4);
}
