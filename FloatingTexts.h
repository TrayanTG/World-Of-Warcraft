#pragma once
#include "HeadersAndConsts.h"

class FloatingTexts
{
	char **text;
	int tlx;
	int tly;
	int length;
	int height;
public:
	FloatingTexts(int length, int height, int tlx = 0, int tly = 0);
	FloatingTexts(const FloatingTexts&) = delete;
	FloatingTexts& operator= (const FloatingTexts&) = delete;
	~FloatingTexts();
	void show(Colour foreground = White, Colour background = Black);
	void update();
	bool addText(const char *text);
};