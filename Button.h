#pragma once
#include "HeadersAndConsts.h"

class Button
{
	int tlx;
	int tly;
	int lenght;
	char *text;
public:
	Button(const char *text, int tlx = -1, int tly = -1);
	Button(const Button &other);
	Button& operator= (const Button &other);
	~Button();

	int getTLX()const;
	int getTLY()const;
	int getLen()const;
	const char *getText()const;

	bool setXY(int tlx, int tly);
	bool showButton();
	bool isWithin(int x, int y)const;
};