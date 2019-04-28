#include "Item.h"

Item::Item(int id, int price, int minLevel, const char *title, const char *description, const char *name, int size,
	bool marked, int tlx, int tly) :
	Box(title, description, name, size, marked, tlx, tly)
{
	this->id = id;
	this->price = price;
	this->minLevel = minLevel;
}

Item::~Item()
{
	//std::cout << "Item destroyed!" << std::endl;
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
