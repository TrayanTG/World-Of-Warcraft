#include "FloatingTexts.h"
#include "Graphics.h"
#include <iostream>

FloatingTexts::FloatingTexts(int length, int height, const COORD &topLeft): topLeft(topLeft)
{
	if (length > MAX_SIZE) throw std::runtime_error("FloatingTexts(int, int, const COORD&) overflow!\n");
	this->length = length;
	this->height = height;
	text.resize(height);
	for (int i = 0;i < height;i++)
	{
		text[i] = emptyLine;
		text[i][length] = 0;
	}
}

void FloatingTexts::show(Colour foreground, Colour background)const
{
	Graphics::getInstance().setcolor(foreground, background);
	for (int i = 0;i < height;i++)
	{
		Graphics::getInstance().gotoxy(topLeft.X, topLeft.Y + height - (i + 1));
		std::cout << text[i];
	}
	Graphics::getInstance().setcolor(White);
}

void FloatingTexts::update()
{
	for (int i = height - 1; i > 0;i--)
	{
		text[i] = text[i - 1];
	}
	text[0] = emptyLine;
	text[0][length] = 0;
}

bool FloatingTexts::addText(const string &text)
{
	if (text.size() > (size_t)length)return false;
	update();
	this->text[0] = text;
	this->text[0] += emptyLine;
	this->text[0][length] = 0;
	return true;
}
