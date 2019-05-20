#pragma once
#include "InfoBox.h"

class Box : protected InfoBox
{
protected:
	string name;
	Coord topLeft;
	int size;
	bool marked;

	int getQuadrant()const;

public:
	Box(const string &title = "", const string &description = "", const string &name = "", int size = 0, const Coord &topLeft = { -1,-1 }, bool marked = false);

	const Coord &getTopLeft()const;
	Coord getInfoBoxTopLeft()const;
	Coord getInfoBoxBotRight()const;

	bool isWithin(const Coord &pos, int range = 0)const;
	bool isMarked()const;
	bool setMarked(bool marked = true);
	bool setXY(const Coord &topLeft);

	virtual int getID()const;
	virtual void showBox(const Damage &damage = { 0,0 })const;
	void hideBox()const;
	void toggleInfoBox()const;
	void hideInfoBox()const;
};
