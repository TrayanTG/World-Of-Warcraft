#include "Player.h"

Player::Player() : Character()
{
	helmet = shoulders = chest = gloves = legs = feet = &Data::getIstance().emptyArmor;
	weapon = &Data::getIstance().emptyWeapon;
	eqAbilities[0] = &Data::getIstance().emptyAbility;
	eqAbilities[1] = &Data::getIstance().emptyAbility;
	eqAbilities[2] = &Data::getIstance().emptyAbility;
	eqAbilities[3] = &Data::getIstance().emptyAbility;

}

bool Player::equipHelmet(Item *eqHelmet)
{
	if (helmet == eqHelmet)return false;
	helmet = (Armor*)eqHelmet;
	return true;
}

bool Player::equipShoulders(Item *eqShoulders)
{
	if (shoulders == eqShoulders)return false;
	shoulders = (Armor*)eqShoulders;
	return true;
}

bool Player::equipChest(Item *eqChest)
{
	if (chest == eqChest)return false;
	chest = (Armor*)eqChest;
	return true;
}

bool Player::equipGloves(Item *eqGloves)
{
	if (gloves == eqGloves)return false;
	gloves = (Armor*)eqGloves;
	return true;
}

bool Player::equipLegs(Item *eqLegs)
{
	if (legs == eqLegs)return false;
	legs = (Armor*)eqLegs;
	return true;
}

bool Player::equipFeet(Item *eqFeet)
{
	if (feet == eqFeet)return false;
	feet = (Armor*)eqFeet;
	return true;
}

bool Player::equipWeapon(Item *eqWeapon)
{
	if (weapon == eqWeapon)return false;
	weapon = (Weapon*)eqWeapon;
	return true;
}

// --------------------------------------------------------

bool Player::loadPlayer(const char *directory)
{
	loadCharacter(directory);
	int temp;
	char path[MAX_PATH_LENGHT];
	strcpy(path, directory);
	std::ifstream iFile(strcat(path, "Abilities"));
	if (!iFile)return false;
	while (iFile)
	{
		iFile >> temp;
		if (iFile && !addAbility(Data::getIstance().getAbilityByID(temp)))return false;
	}
	iFile.close();
	path[strlen(directory)] = 0;
	iFile.open(strcat(path, "Items"));
	if (!iFile)return false;
	while (iFile)
	{
		iFile >> temp;
		if (iFile && !buyItem(Data::getIstance().getItemByID(temp)))return false;
	}
	iFile.close();
	path[strlen(directory)] = 0;
	iFile.open(strcat(path, "EquippedItems"));
	if (!iFile)return false;
	while (iFile)
	{
		iFile >> temp;
		if (iFile && !equipItem(Data::getIstance().getItemByID(temp))) return false;
	}
	iFile.close();
	path[strlen(directory)] = 0;
	iFile.open(strcat(path, "EquippedAbilities"));
	if (!iFile)return false;
	int index = 0;
	while (iFile)
	{
		iFile >> temp;
		if (temp == -1)
		{
			index++;
			continue;
		}
		if (iFile && !equipAbility(Data::getIstance().getAbilityByID(temp), index++)) return false;
	}
	iFile.close();
	return true;
}

bool Player::savePlayer(const char *directory)
{
	//_mkdir(directory);
	saveCharacter(directory);
	char path[MAX_PATH_LENGHT];
	strcpy(path, directory);
	std::ofstream oFile(strcat(path, "Abilities"));
	if (!oFile) return false;
	for (size_t i = 0;i < abilities.size();i++)oFile << abilities[i]->getID() << '\n';
	oFile.close();
	path[strlen(directory)] = 0;
	oFile.open(strcat(path, "Items"));
	if (!oFile)return false;
	for (size_t i = 0; i < items.size();i++)oFile << items[i]->getID() << '\n';
	oFile.close();
	path[strlen(directory)] = 0;
	oFile.open(strcat(path, "EquippedItems"));
	if (!oFile) return false;
	if (weapon != nullptr) oFile << weapon->getID() << '\n';
	if (helmet != nullptr) oFile << helmet->getID() << '\n';
	if (shoulders != nullptr) oFile << shoulders->getID() << '\n';
	if (chest != nullptr) oFile << chest->getID() << '\n';
	if (gloves != nullptr) oFile << gloves->getID() << '\n';
	if (legs != nullptr) oFile << legs->getID() << '\n';
	if (feet != nullptr) oFile << feet->getID() << '\n';
	oFile.close();
	path[strlen(directory)] = 0;
	oFile.open(strcat(path, "EquippedAbilities"));
	if (!oFile)return false;
	if (eqAbilities[0] == nullptr) oFile << -1 << '\n';else oFile << eqAbilities[0]->getID() << '\n';
	if (eqAbilities[1] == nullptr) oFile << -1 << '\n';else oFile << eqAbilities[1]->getID() << '\n';
	if (eqAbilities[2] == nullptr) oFile << -1 << '\n';else oFile << eqAbilities[2]->getID() << '\n';
	if (eqAbilities[3] == nullptr) oFile << -1 << '\n';else oFile << eqAbilities[3]->getID() << '\n';
	oFile.close();
	return true;
}

bool Player::buyItem(Item *newItem)
{
	auto it = find(items.begin(), items.end(), newItem);
	if (it != items.end()) return false;
	items.push_back(newItem);
	return true;
}

bool Player::sellItem(Item *soldItem)
{
	auto it = find(items.begin(), items.end(), soldItem);
	if (it == items.end()) return false;
	items.erase(it);
	return true;
}

bool Player::addAbility(Ability *newAbility)
{
	auto it = find(abilities.begin(), abilities.end(), newAbility);
	if (it != abilities.end()) return false;
	abilities.push_back(newAbility);
	return true;
}

bool Player::equipItem(Item *eqItem)
{
	auto it = find(items.begin(), items.end(), eqItem);
	if (it == items.end()) return false;
	if (typeid(*eqItem) == typeid(Weapon)) return equipWeapon(eqItem);
	Armor *eqArmor = (Armor*)eqItem;
	switch (eqArmor->getArmorType())
	{
		case Helmet: return equipHelmet(eqItem); break;
		case Shoulders: return equipHelmet(eqItem); break;
		case Chest: return equipHelmet(eqItem); break;
		case Gloves: return equipHelmet(eqItem); break;
		case Legs: return equipHelmet(eqItem); break;
		case Feet: return equipHelmet(eqItem); break;
	}
	return false;
}

bool Player::equipAbility(Ability *eqAbility, int slot)
{
	if (eqAbilities[0] == eqAbility || eqAbilities[1] == eqAbility || eqAbilities[2] == eqAbility || eqAbilities[3] == eqAbility) return false;
	auto it = find(abilities.begin(), abilities.end(), eqAbility);
	if (it == abilities.end()) return false;
	eqAbilities[slot] = eqAbility;
	return true;
}

Damage Player::getAbilityDamage(int slot)const
{
	int power = abilities[slot]->getPower();
	if (power == -1)return -1;
	return { ((power + 100)*getTotalDamageStats().Physical) / 100, ((power + 100)*getTotalDamageStats().Magical) / 100 };
}

Damage Player::dealDamage(int slot)const
{
	return getAbilityDamage(slot);
}


int Player::calcDamage(const Damage &damage)const
{
	int physical = damage.Physical - getTotalDefenceStats().Armor;
	int magical = damage.Magical - getTotalDefenceStats().MagicResist;
	if (physical < 0)physical = 0;
	if (magical < 0)magical = 0;
	return physical + magical;
}

Damage Player::getTotalDamageStats()const
{
	Damage damage = baseDamage;
	damage += weapon->getDamageStats();
	return damage;
}

Defence Player::getTotalDefenceStats()const
{
	Defence defence = baseDefence;
	defence += helmet->getDefenceStats();
	defence += shoulders->getDefenceStats();
	defence += chest->getDefenceStats();
	defence += gloves->getDefenceStats();
	defence += legs->getDefenceStats();
	defence += feet->getDefenceStats();
	return defence;
}
