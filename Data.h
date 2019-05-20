#pragma once
#include "HeadersAndConsts.h"
#include "Weapon.h"
#include "Armor.h"
#include "Ability.h"
#include "Enemy.h"

class Data
{
	Data();

	static std::vector<Item*> items;
	static std::vector<Ability*> abilities;
	static std::vector<Enemy*> enemies;

	static const Weapon emptyWeapon;
	static const Armor emptyArmor;
	static const Ability emptyAbility;

public:
	Data(const Data&) = delete;
	Data& operator=(const Data&) = delete;

	~Data();

	static size_t getItemCount();
	static size_t getAbilityCount();
	static size_t getEnemyCount();

	static const Item &getItem(int index);
	static const Ability &getAbility(int index);
	static const Enemy &getEnemy(int index);

	static const Item &getItemByID(int id);
	static const Ability &getAbilityByID(int id);
	static const Enemy &getEnemyByID(int id);
	
	static const Armor &getEmptyArmor();
	static const Weapon &getEmptyWeapon();
	static const Ability &getEmptyAbility();

	static void loadEnemies(const string &directory = "Data/Enemies/");
	static void loadItems(const string &directory = "Data/Items/");
	static void loadAbilities(const string &directory = "Data/Abilities/");
	static void addEnemy(std::ifstream &iFile);
	static void addItem(std::ifstream &iFile);
	static void addAbility(std::ifstream &iFile);
};