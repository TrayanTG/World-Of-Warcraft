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
	Item(int id = -1, int price = 0, int minLevel = 0, const char *title = "", const char *description = "", 
		const char *name = "", int size = DEF_ITEM_SIZE, bool marked = false, int tlx = -1, int tly = -1);
	Item(const Item&) = delete;
	virtual ~Item();

	virtual Damage getDamageStats()const = 0;
	virtual Defence getDefenceStats()const = 0;

	int getID()const;
	int getPrice()const;
	int getMinLevel()const;
};