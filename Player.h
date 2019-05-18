#pragma once
#include "Character.h"
#include "Weapon.h"
#include "Ability.h"
#include "Armor.h"

class Player : public Character
{
	bool equipHelmet(Item *eqHelmet);
	bool equipShoulders(Item *eqShoulders);
	bool equipChest(Item *eqChest);
	bool equipGloves(Item *eqGloves);
	bool equipLegs(Item *eqLegs);
	bool equipFeet(Item *eqFeet);
	bool equipWeapon(Item *eqWeapon);

	void init();

public:
	virtual ~Player() {}

	std::vector<Item*> items;
	std::vector<Ability*> abilities;
	Armor *helmet, *shoulders, *chest, *gloves, *legs, *feet;
	Weapon *weapon;
	Ability *eqAbilities[4];

	Player();

	bool loadPlayer(const char *directory);
	bool savePlayer(const char *directory);

	bool buyItem(Item *newItem, bool isFree = false);
	bool sellItem(Item *soldItem);
	bool addAbility(Ability *newAbility);
	bool equipItem(Item *eqItem);
	bool equipAbility(Ability *eqAbility, int slot);

	virtual void regenRes() = 0;
	virtual bool levelUp();
	virtual bool gainDamage(const Damage &damage);
	virtual int calcDamage(const Damage &damage)const;
	virtual Damage dealDamage(int slot);
	virtual Damage getTotalDamageStats()const;
	virtual Defence getTotalDefenceStats()const;
};
