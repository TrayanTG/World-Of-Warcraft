#include "Item.h"

Item::Item(int id, int price, int minLevel, const string &title, const string &description, const string &name, int size, 
	const Coord &topLeft, bool marked):
	Box(title, description, name, size, topLeft, marked)
{
	this->id = id;
	this->price = price;
	this->minLevel = minLevel;
}

Item::~Item()
{
	
}

int Item::getID()const
{
	return id;
}

int Item::getPrice()const
{
	return price;
}

int Item::getMinLevel()const
{
	return minLevel;
}
