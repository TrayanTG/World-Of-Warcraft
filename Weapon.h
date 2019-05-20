#pragma once
#include "Item.h"

class Weapon : public Item
{
	Damage damage;
	WeaponType weaponType;
public:
	Weapon(int id = -1, int price = 0, int minLevel = 0, const Damage &damage = { 0, 0 }, 
		WeaponType weaponType = uWeaponType, const string &title = "", const string &description = "",
		const string &name = "", int size = DEF_ITEM_SIZE, const Coord &topLeft = { -1,-1 }, bool marked = false);
	Weapon(std::ifstream &iFile);

	WeaponType getWeaponType()const;
	virtual void showBox(const Damage &damage = { 0,0 })const override;
	virtual Damage getDamageStats()const override;
	virtual Defence getDefenceStats()const override;
	virtual Item* clone()const override;
};