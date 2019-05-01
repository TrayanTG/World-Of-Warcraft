#pragma once
#include "Item.h"

class Weapon : public Item
{
	Damage damage;
	WeaponType weaponType;
public:
	Weapon(int id = -1, int price = 0, int minLevel = 0, const Damage &damage = { 0, 0 }, 
		WeaponType weaponType = uWeaponType, const char *title = "", const char *description = "",
		const char *name = "", int size = DEF_ITEM_SIZE, bool marked = false, int tlx = -1, int tly = -1);
	Weapon(const Weapon&) = delete;
	Weapon(std::ifstream &iFile);

	WeaponType getWeaponType()const;
	virtual bool showBox(const Damage &damage = { 0,0 })const;
	virtual Damage getDamageStats()const;
	virtual Defence getDefenceStats()const;
};