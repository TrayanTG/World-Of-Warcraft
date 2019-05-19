#pragma once
#include "InfoBox.h"

class Box : protected InfoBox
{
protected:
	string name;
	COORD topLeft;
	int size;
	bool marked;

	int getQuadrant()const;

public:
	Box(const string &title = "", const string &description = "", const string &name = "", int size = 0, const COORD &topLeft = { -1,-1 }, bool marked = false);

	const COORD &getTopLeft()const;
	const COORD &getInfoBoxTopLeft()const;
	const COORD &getInfoBoxBotRight()const;

	bool isWithin(const COORD &pos, int range = 0)const;
	bool isMarked()const;
	bool setMarked(bool marked = true);
	bool setXY(const COORD &topLeft);

	virtual int getID()const;
	virtual void showBox(const Damage &damage = { 0,0 })const;
	void hideBox()const;
	void toggleInfoBox()const;
	void hideInfoBox()const;
};
