#pragma once
#include "HeadersAndConsts.h"

class Button
{
	string text;
	Coord topLeft;
public:
	Button(const string &text = "", const Coord &topLeft = { -1,-1 });

	const Coord &getTopLeft()const;
	int getLen()const;
	const string &getText()const;

	bool setXY(const Coord &topLeft);
	void showButton();
	bool isWithin(const Coord &pos)const;
};