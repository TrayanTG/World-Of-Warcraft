#pragma once
#include "Item.h"

class Armor : public Item
{
	Defence defence;
	ArmorType armorType;
public:
	Armor(int id = -1, int price = 0, int minLevel = 0, const Defence &defence = { 0, 0 }, 
		ArmorType armorType = uArmorType, const char *title = "", const char *description = "",
		const char *name = "", int size = DEF_ITEM_SIZE, bool marked = false, int tlx = -1, int tly = -1);
	Armor(const Armor&) = delete;
	Armor(std::ifstream &iFile);

	ArmorType getArmorType()const;
	virtual bool showBox(const Damage &damage = { 0,0 })const;
	virtual Damage getDamageStats()const;
	virtual Defence getDefenceStats()const;
};