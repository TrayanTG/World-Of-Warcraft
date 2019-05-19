#pragma once
#include "Item.h"

class Armor : public Item
{
	Defence defence;
	ArmorType armorType;
public:
	Armor(int id = -1, int price = 0, int minLevel = 0, const Defence &defence = { 0, 0 }, 
		ArmorType armorType = uArmorType, const string &title = "", const string &description = "",
		const string &name = "", int size = DEF_ITEM_SIZE, const COORD &topLeft = { -1,-1 }, bool marked = false);
	Armor(std::ifstream &iFile);

	ArmorType getArmorType()const;
	virtual void showBox(const Damage &damage = { 0,0 })const override;
	virtual Damage getDamageStats()const override;
	virtual Defence getDefenceStats()const override;
	virtual Item* clone()const override;
};