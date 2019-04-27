#pragma once
#include "HeadersAndConsts.h"
#include "Weapon.h"
#include "Armor.h"
#include "Ability.h"

class Data
{
	static Data s;

	Data();

public:

	Data(const Data&) = delete;
	Data& operator=(const Data&) = delete;

	~Data();

	static Data& getIstance();

	std::vector<Item*> items;
	std::vector<Ability*> abilities;

	Item *getItemByID(int id);
	Ability *getAbilityByID(int id);
	Weapon emptyWeapon;
	Armor emptyArmor;
	Ability emptyAbility;

	void loadItems(const char *directory = "Data/Items/");
	void loadAbilities(const char *directory = "Data/Abilities/");
	void addItem(std::ifstream &iFile);
	void addAbility(std::ifstream &iFile);
};