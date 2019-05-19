#pragma once
#include "HeadersAndConsts.h"

using std::vector;

class FloatingTexts
{
	static const string emptyLine;
	static const int MAX_SIZE;
	vector<string> text;
	COORD topLeft;
	int length;
	int height;
public:
	FloatingTexts(int length, int height, const COORD &topLeft);
	void show(Colour foreground = White, Colour background = Black)const;
	void update();
	bool addText(const string &text);
};

const string FloatingTexts::emptyLine = "                                    ";
const int FloatingTexts::MAX_SIZE = 36;