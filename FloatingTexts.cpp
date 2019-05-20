#include "FloatingTexts.h"
#include "Graphics.h"
#include <iostream>

int index = 0;

const string FloatingTexts::emptyLine = "                                    ";
const int FloatingTexts::MAX_LINE_SIZE = 36;

FloatingTexts::FloatingTexts(int length, int height, const Coord &topLeft): topLeft(topLeft)
{
	if (length > MAX_LINE_SIZE) throw std::runtime_error("FloatingTexts(int, int, const Coord&) overflow!\n");
	this->length = length;
	this->height = height;
	text.resize(height);
	for (int i = 0;i < height;i++)
	{
		text[i] = emptyLine;
		text[i] = text[i].substr(0, length);
	}
}

void FloatingTexts::show(Colour foreground, Colour background)const
{
	Graphics::setcolor(foreground, background);
	for (int i = 0;i < height;i++)
	{
		Graphics::gotoxy(topLeft.X, topLeft.Y + height - (i + 1));
		std::cout << text[i];
	}
	Graphics::setcolor(White);
}

void FloatingTexts::update()
{
	for (int i = height - 1; i > 0;i--)
	{
		text[i] = text[i - 1];
	}
	text[0] = emptyLine;
	text[0] = text[0].substr(0, length);
}

bool FloatingTexts::addText(const string &text)
{
	if (text.size() > (size_t)length)return false;
	update();
	this->text[0] = text;
	this->text[0] += emptyLine;
	this->text[0] = this->text[0].substr(0, length);
	return true;
}
