#pragma once
#include "HeadersAndConsts.h"

class InfoBox
{
	char *title;
	char *description;

protected:
	int getRows(int width)const;

public:
	InfoBox(const char *title = "", const char *description = "");
	InfoBox(const InfoBox &other) = delete;
	InfoBox& operator= (const InfoBox &other);
	~InfoBox();

	bool showInfoBox(int tlx, int tly, int brx, int bry)const;
	bool clearInfoBox(int tlx, int tly, int brx, int bry)const;
};