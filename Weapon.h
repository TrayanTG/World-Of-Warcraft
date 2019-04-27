#pragma once
#include "Item.h"

class Weapon : public Item
{
	Damage damage;
	WeaponType weaponType;
public:
	Weapon(int id = 0, int price = 0, int minLevel = 0, const Damage &damage = { 0, 0 }, 
		WeaponType weaponType = uWeaponType, const char *title = "", const char *description = "",
		const char *name = "", int size = 0, bool marked = false, int tlx = -1, int tly = -1);
	Weapon(const Weapon&) = delete;
	Weapon(std::ifstream &iFile);

	virtual bool showBox()const;
	virtual Damage getDamage()const;
	virtual Defence getDefence()const;
};