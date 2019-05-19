#pragma once
#include "InfoBox.h"

class Box : protected InfoBox
{
protected:
	char *name;
	int size;
	int tlx;
	int tly;
	bool marked;

	int getQuadrant()const;

public:
	Box(const char *title = "", const char *description = "", const char *name = "", int size = 0, bool marked = false, int tlx = -1, int tly = -1);
	Box(const Box&) = delete;
	Box& operator= (const Box &other);
	~Box();

	int getTLX()const;
	int getTLY()const;
	int ibTLX()const;
	int ibTLY()const;
	int ibBRX()const;
	int ibBRY()const;

	bool isWithin(int x, int y, int range = 0)const; //not tested
	bool isMarked()const;
	bool setMarked(bool marked = true);
	bool setXY(int x, int y);

	virtual int getID()const;
	virtual bool showBox(const Damage &damage = { 0,0 })const;
	bool hideBox()const;
	bool toggleInfoBox()const;
	bool hideInfoBox()const;
};
