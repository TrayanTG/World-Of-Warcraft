#pragma once
#include "HeadersAndConsts.h"
#include "Box.h"
#include "Damage.h"
#include "Defence.h"

class Item: public Box
{
protected:
	int id;
	int price;
	int minLevel;

public:
	Item(int id = -1, int price = 0, int minLevel = 0, const string &title = "", const string &description = "", 
		const string &name = "", int size = DEF_ITEM_SIZE, const Coord &topLeft = { -1,-1 }, bool marked = false);
	virtual ~Item();

	int getID()const;
	int getPrice()const;
	int getMinLevel()const;

	virtual Item* clone()const = 0;
	virtual Damage getDamageStats()const = 0;
	virtual Defence getDefenceStats()const = 0;
};