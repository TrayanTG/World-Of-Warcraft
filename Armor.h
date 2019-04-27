#pragma once
#include "Item.h"

class Armor : public Item
{
	Defence defence;
	ArmorType armorType;
public:
	Armor(int id = 0, int price = 0, int minLevel = 0, const Defence &defence = { 0, 0 }, 
		ArmorType armorType = uArmorType, const char *title = "", const char *description = "",
		const char *name = "", int size = 0, bool marked = false, int tlx = -1, int tly = -1);
	Armor(const Armor&) = delete;
	Armor(std::ifstream &iFile);

	virtual bool showBox()const;
	virtual Damage getDamage()const;
	virtual Defence getDefence()const;
};