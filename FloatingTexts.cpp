#include "FloatingTexts.h"
#include "Graphics.h"
#include <iostream>

FloatingTexts::FloatingTexts(int length, int height, int tlx, int tly)
{
	if (length < 0 || height < 0 || tlx < 0 || tly < 0 || length + tlx >= DEF_CONSOLE_WIDTH || height + tly >= DEF_CHARACTER_HEIGHT)
	{
		throw std::runtime_error("Floating text format error!");
	}
	this->length = length;
	this->height = height;
	this->tlx = tlx;
	this->tly = tly;
	text = new char*[height];
	for (int i = 0;i < height;i++)
	{
		text[i] = new char[length + 1];
		strncpy(text[i], "                                    ", length);
		text[i][length] = 0;
	}
}

FloatingTexts::~FloatingTexts()
{
	for (int i = 0;i < height;i++)
	{
		delete[] text[i];
	}
	delete[] text;
}

void FloatingTexts::show(Colour foreground, Colour background)
{
	Graphics::getInstance().setcolor(foreground, background);
	for (int i = 0;i < height;i++)
	{
		Graphics::getInstance().gotoxy(tlx, tly + height - (i + 1));
		std::cout << text[i];
	}
	Graphics::getInstance().setcolor(White);
}

void FloatingTexts::update()
{
	for (int i = height - 1; i > 0;i--)
	{
		strcpy(text[i], text[i - 1]);
	}
	strncpy(text[0], "                                    ", length);
}

bool FloatingTexts::addText(const char *text)
{
	if (strlen(text) > (size_t)length)return false;
	update();
	strcpy(this->text[0], text);
	strncat(this->text[0], "                                    ", length - strlen(this->text[0]));
	this->text[0][length] = 0;
	return true;
}
