#pragma once
#include "HeadersAndConsts.h"
#include "Graphics.h"
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
	Item(int id = 0, int price = 0, int minLevel = 0, const char *title = "", const char *description = "", 
		const char *name = "", int size = 0, bool marked = false, int tlx = -1, int tly = -1);
	Item(const Item&) = delete;
	virtual ~Item();

	virtual Damage getDamage()const = 0;
	virtual Defence getDefence()const = 0;

	int getID()const;
	int getPrice()const;
	int getMinLevel()const;
};