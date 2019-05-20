#pragma once
#include "HeadersAndConsts.h"

using std::vector;

class FloatingTexts
{
	static const string emptyLine;
	static const int MAX_LINE_SIZE;
	vector<string> text;
	Coord topLeft;
	int length;
	int height;
public:
	FloatingTexts(int length, int height, const Coord &topLeft);
	void show(Colour foreground = White, Colour background = Black)const;
	void update();
	bool addText(const string &text);
};
