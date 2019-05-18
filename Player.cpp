#include "Player.h"
#include "Data.h"

Player::Player() : Character()
{
	init();
	buyItem(Data::getIstance().getItemByID(DEF_STARTER_LEGGINGS_ID), true);
	equipItem(Data::getIstance().getItemByID(DEF_STARTER_LEGGINGS_ID));
	addAbility(Data::getIstance().getAbilityByID(DEF_ABILITY_ID));
	equipAbility(Data::getIstance().getAbilityByID(DEF_ABILITY_ID), DEF_ABILITY_SLOT);
	transferDefenceToHP();
}

void Player::init()
{
	abilities.clear();
	items.clear();
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
	if (helmet->getID() >= 0) defence -= helmet->getDefenceStats();
	defence += eqHelmet->getDefenceStats();
	helmet = (Armor*)eqHelmet;
	transferDefenceToHP();
	return true;
}

bool Player::equipShoulders(Item *eqShoulders)
{
	if (shoulders == eqShoulders)return false;
	if (shoulders->getID() >= 0) defence -= shoulders->getDefenceStats();
	defence += eqShoulders->getDefenceStats();
	shoulders = (Armor*)eqShoulders;
	transferDefenceToHP();
	return true;
}

bool Player::equipChest(Item *eqChest)
{
	if (chest == eqChest)return false;
	if (chest->getID() >= 0) defence -= chest->getDefenceStats();
	defence += eqChest->getDefenceStats();
	chest = (Armor*)eqChest;
	transferDefenceToHP();
	return true;
}

bool Player::equipGloves(Item *eqGloves)
{
	if (gloves == eqGloves)return false;
	if (gloves->getID() >= 0) defence -= gloves->getDefenceStats();
	defence += eqGloves->getDefenceStats();
	gloves = (Armor*)eqGloves;
	transferDefenceToHP();
	return true;
}

bool Player::equipLegs(Item *eqLegs)
{
	if (legs == eqLegs)return false;
	if (legs->getID() >= 0) defence -= legs->getDefenceStats();
	defence += eqLegs->getDefenceStats();
	legs = (Armor*)eqLegs;
	transferDefenceToHP();
	return true;
}

bool Player::equipFeet(Item *eqFeet)
{
	if (feet == eqFeet)return false;
	if (feet->getID() >= 0) defence -= feet->getDefenceStats();
	defence += eqFeet->getDefenceStats();
	feet = (Armor*)eqFeet;
	transferDefenceToHP();
	return true;
}

bool Player::equipWeapon(Item *eqWeapon)
{
	if (weapon == eqWeapon)return false;
	if (weapon->getID() >= 0) damage -= weapon->getDamageStats();
	damage += eqWeapon->getDamageStats();
	weapon = (Weapon*)eqWeapon;
	return true;
}

// --------------------------------------------------------

bool Player::loadPlayer(const char *directory)
{
	init();
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
		
		iFile >> temp;//std::cout << temp << std::endl;
		if (iFile && !buyItem(Data::getIstance().getItemByID(temp), true))return false;
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
	damage -= weapon->getDamageStats();
	defence -= helmet->getDefenceStats();
	defence -= chest->getDefenceStats();
	defence -= shoulders->getDefenceStats();
	defence -= gloves->getDefenceStats();
	defence -= legs->getDefenceStats();
	defence -= feet->getDefenceStats();
	saveCharacter(directory);
	damage += weapon->getDamageStats();
	defence += helmet->getDefenceStats();
	defence += chest->getDefenceStats();
	defence += shoulders->getDefenceStats();
	defence += gloves->getDefenceStats();
	defence += legs->getDefenceStats();
	defence += feet->getDefenceStats();
	char path[MAX_PATH_LENGHT];
	strcpy(path, directory);
	std::ofstream oFile(strcat(path, "Abilities"));
	if (!oFile) return false;
	for (size_t i = 0;i < abilities.size();i++)
	{
		if (abilities[i]->getID() >= 0)
		{
			oFile << abilities[i]->getID() << '\n';
		}
	}
	oFile.close();
	path[strlen(directory)] = 0;
	oFile.open(strcat(path, "Items"));
	if (!oFile)return false;
	for (size_t i = 0; i < items.size();i++)
	{

		if (items[i]->getID() >= 0)
		{
			oFile << items[i]->getID() << '\n';
		}
	}
	oFile.close();
	path[strlen(directory)] = 0;
	oFile.open(strcat(path, "EquippedItems"));
	if (!oFile) return false;
	if (weapon->getID() >= 0) oFile << weapon->getID() << '\n';
	if (helmet->getID() >= 0) oFile << helmet->getID() << '\n';
	if (shoulders->getID() >= 0) oFile << shoulders->getID() << '\n';
	if (chest->getID() >= 0) oFile << chest->getID() << '\n';
	if (gloves->getID() >= 0) oFile << gloves->getID() << '\n';
	if (legs->getID() >= 0) oFile << legs->getID() << '\n';
	if (feet->getID() >= 0) oFile << feet->getID() << '\n';
	oFile.close();
	path[strlen(directory)] = 0;
	oFile.open(strcat(path, "EquippedAbilities"));
	if (!oFile)return false;
	oFile << eqAbilities[0]->getID() << '\n';
	oFile << eqAbilities[1]->getID() << '\n';
	oFile << eqAbilities[2]->getID() << '\n';
	oFile << eqAbilities[3]->getID() << '\n';
	oFile.close();
	return true;
}

bool Player::buyItem(Item *newItem, bool isFree)
{
	if (!isFree && gold < newItem->getPrice())return false;
	auto it = find(items.begin(), items.end(), newItem);
	if (it != items.end()) return false;
	items.push_back(newItem);
	if (!isFree)gold -= newItem->getPrice();
	return true;
}

bool Player::sellItem(Item *soldItem)
{
	auto it = find(items.begin(), items.end(), soldItem);
	if (it == items.end()) return false;
	items.erase(it);
	gainGold(soldItem->getPrice()/2);
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
		case Shoulders: return equipShoulders(eqItem); break;
		case Chest: return equipChest(eqItem); break;
		case Gloves: return equipGloves(eqItem); break;
		case Legs: return equipLegs(eqItem); break;
		case Feet: return equipFeet(eqItem); break;
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

Damage Player::dealDamage(int slot)
{
	int power = eqAbilities[slot]->getPower();
	if (power == -1)return { 0,0 };
	return { ((power + 100)*getTotalDamageStats().Physical) / 100, ((power + 100)*getTotalDamageStats().Magical) / 100 };
}

bool Player::levelUp()
{
	if (!Character::levelUp())return false;

	for (size_t i = 0;i < Data::getIstance().abilities.size();i++)
	{
		if (Data::getIstance().abilities[i]->getMinLevel() == level)
		{
			addAbility(Data::getIstance().abilities[i]);
		}
	}
	return true;
}

bool Player::gainDamage(const Damage &damage)
{
	HP.decreaseBy(calcDamage(damage));
	return isAlive();
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
	return damage;
}

Defence Player::getTotalDefenceStats()const
{
	return defence;
}
