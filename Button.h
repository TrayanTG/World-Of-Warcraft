#pragma once
#include "HeadersAndConsts.h"

class Button
{
	string text;
	COORD topLeft;
public:
	Button(const string &text = "", const COORD &topLeft = { -1,-1 });

	const COORD &getTopLeft()const;
	int getLen()const;
	const string &getText()const;

	bool setXY(const COORD &topLeft);
	void showButton();
	bool isWithin(const COORD &pos)const;
};