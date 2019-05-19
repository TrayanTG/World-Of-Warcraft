#pragma once
#include "HeadersAndConsts.h"

class InfoBox
{
protected:
	string title;
	string description;

	int getRows(int width)const;

public:
	InfoBox(const string &title = "", const string &description = "");

	bool showInfoBox(const COORD &topLeft, const COORD &botRight)const;
	bool clearInfoBox(const COORD &topLeft, const COORD &botRight)const;
};