#include "Player.h"
#include "Data.h"

Player::Player()// : Character()
{
	init();
	buyItem(Data::getItemByID(DEF_STARTER_LEGGINGS_ID), true);
	equipItem(Data::getItemByID(DEF_STARTER_LEGGINGS_ID));
	addAbility(Data::getAbilityByID(DEF_ABILITY_ID));
	equipAbility(Data::getAbilityByID(DEF_ABILITY_ID), DEF_ABILITY_SLOT);
	transferDefenceToHP();
}

void Player::init()
{
	abilities.clear();
	items.clear();
	helmet = shoulders = chest = gloves = legs = feet = Data::getEmptyArmor();
	weapon = Data::getEmptyWeapon();
	eqAbilities[0] = Data::getEmptyAbility();
	eqAbilities[1] = Data::getEmptyAbility();
	eqAbilities[2] = Data::getEmptyAbility();
	eqAbilities[3] = Data::getEmptyAbility();
}

const Item &Player::getItem(int index)const
{
	return *items[index];
}

const Ability &Player::getAbility(int index)const
{
	return *abilities[index];
}

const Ability &Player::getEqAbility(int index)const
{
	if (index >= 4)throw std::runtime_error("Ivalid Equipped Ability Slot!\n");
	return eqAbilities[index];
}

const Armor &Player::getHelmet()const
{
	return helmet;
}

const Armor &Player::getShoulders()const
{
	return shoulders;
}

const Armor &Player::getChest()const
{
	return chest;
}

const Armor &Player::getGloves()const
{
	return gloves;
}

const Armor &Player::getLegs()const
{
	return legs;
}

const Armor &Player::getFeet()const
{
	return feet;
}

const Weapon &Player::getWeapon()const
{
	return weapon;
}

bool Player::equipHelmet(const Item &eqHelmet)
{
	if (helmet.getID() == eqHelmet.getID())return false;
	if (helmet.getID() >= 0) defence -= helmet.getDefenceStats();
	defence += eqHelmet.getDefenceStats();
	helmet = (Armor&)eqHelmet;
	transferDefenceToHP();
	return true;
}

bool Player::equipShoulders(const Item &eqShoulders)
{
	if (shoulders.getID() == eqShoulders.getID())return false;
	if (shoulders.getID() >= 0) defence -= shoulders.getDefenceStats();
	defence += eqShoulders.getDefenceStats();
	shoulders = (Armor&)eqShoulders;
	transferDefenceToHP();
	return true;
}

bool Player::equipChest(const Item &eqChest)
{
	if (chest.getID() == eqChest.getID())return false;
	if (chest.getID() >= 0) defence -= chest.getDefenceStats();
	defence += eqChest.getDefenceStats();
	chest = (Armor&)eqChest;
	transferDefenceToHP();
	return true;
}

bool Player::equipGloves(const Item &eqGloves)
{
	if (gloves.getID() == eqGloves.getID())return false;
	if (gloves.getID() >= 0) defence -= gloves.getDefenceStats();
	defence += eqGloves.getDefenceStats();
	gloves = (Armor&)eqGloves;
	transferDefenceToHP();
	return true;
}

bool Player::equipLegs(const Item &eqLegs)
{
	if (legs.getID() == eqLegs.getID())return false;
	if (legs.getID() >= 0) defence -= legs.getDefenceStats();
	defence += eqLegs.getDefenceStats();
	legs = (Armor&)eqLegs;
	transferDefenceToHP();
	return true;
}

bool Player::equipFeet(const Item &eqFeet)
{
	if (feet.getID() == eqFeet.getID())return false;
	if (feet.getID() >= 0) defence -= feet.getDefenceStats();
	defence += eqFeet.getDefenceStats();
	feet = (Armor&)eqFeet;
	transferDefenceToHP();
	return true;
}

bool Player::equipWeapon(const Item &eqWeapon)
{
	if (weapon.getID() == eqWeapon.getID())return false;
	if (weapon.getID() >= 0) damage -= weapon.getDamageStats();
	damage += eqWeapon.getDamageStats();
	weapon = (Weapon&)eqWeapon;
	return true;
}

// --------------------------------------------------------

bool Player::loadPlayer(const string &directory)
{
	init();
	loadCharacter(directory);
	int temp;
	std::ifstream iFile(directory + "Abilities");
	if (!iFile)return false;
	while (iFile)
	{
		iFile >> temp;
		if (iFile && !addAbility(Data::getAbilityByID(temp)))return false;
	}
	iFile.close();
	iFile.open(directory + "Items");
	if (!iFile)return false;
	while (iFile)
	{
		
		iFile >> temp;
		if (iFile && !buyItem(Data::getItemByID(temp), true))return false;
	}
	iFile.close();
	iFile.open(directory + "EquippedItems");
	if (!iFile)return false;
	while (iFile)
	{
		iFile >> temp;
		if (iFile && !equipItem(Data::getItemByID(temp))) return false;
	}
	iFile.close();
	iFile.open(directory + "EquippedAbilities");
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
		if (iFile && !equipAbility(Data::getAbilityByID(temp), index++)) return false;
	}
	iFile.close();
	return true;
}

bool Player::savePlayer(const string &directory)
{
	//_mkdir(directory);
	damage -= weapon.getDamageStats();
	defence -= helmet.getDefenceStats();
	defence -= chest.getDefenceStats();
	defence -= shoulders.getDefenceStats();
	defence -= gloves.getDefenceStats();
	defence -= legs.getDefenceStats();
	defence -= feet.getDefenceStats();
	saveCharacter(directory);
	damage += weapon.getDamageStats();
	defence += helmet.getDefenceStats();
	defence += chest.getDefenceStats();
	defence += shoulders.getDefenceStats();
	defence += gloves.getDefenceStats();
	defence += legs.getDefenceStats();
	defence += feet.getDefenceStats();

	std::ofstream oFile(directory + "Abilities");
	if (!oFile) return false;
	for (size_t i = 0;i < abilities.size();i++)
	{
		if (abilities[i]->getID() >= 0)
		{
			oFile << abilities[i]->getID() << '\n';
		}
	}
	oFile.close();
	oFile.open(directory + "Items");
	if (!oFile)return false;
	for (size_t i = 0; i < items.size();i++)
	{

		if (items[i]->getID() >= 0)
		{
			oFile << items[i]->getID() << '\n';
		}
	}
	oFile.close();
	oFile.open(directory + "EquippedItems");
	if (!oFile) return false;
	if (weapon.getID() >= 0) oFile << weapon.getID() << '\n';
	if (helmet.getID() >= 0) oFile << helmet.getID() << '\n';
	if (shoulders.getID() >= 0) oFile << shoulders.getID() << '\n';
	if (chest.getID() >= 0) oFile << chest.getID() << '\n';
	if (gloves.getID() >= 0) oFile << gloves.getID() << '\n';
	if (legs.getID() >= 0) oFile << legs.getID() << '\n';
	if (feet.getID() >= 0) oFile << feet.getID() << '\n';
	oFile.close();
	oFile.open(directory + "EquippedAbilities");
	if (!oFile)return false;
	oFile << eqAbilities[0].getID() << '\n';
	oFile << eqAbilities[1].getID() << '\n';
	oFile << eqAbilities[2].getID() << '\n';
	oFile << eqAbilities[3].getID() << '\n';
	oFile.close();
	return true;
}

bool Player::buyItem(const Item &newItem, bool isFree)
{
	if (!isFree && gold < newItem.getPrice())return false;
	for (size_t i = 0; i < items.size();i++)
	{
		if (items[i]->getID() == newItem.getID())return false;
	}
	items.push_back(newItem.clone());
	if (!isFree)gold -= newItem.getPrice();
	return true;
}

bool Player::sellItem(const Item &soldItem)
{
	int i;
	for (size_t i = 0;i < items.size();i++)
	{
		if (items[i]->getID() == soldItem.getID())break;
	}
	if (i == items.size()) return false;
	items.erase(items.begin()+i);  //CHECK
	gainGold(soldItem.getPrice()/2);
	return true;
}

bool Player::addAbility(const Ability &newAbility)
{
	for (size_t i = 0;i < abilities.size();i++)
	{
		if (abilities[i]->getID() == newAbility.getID()) return false;
	}
	abilities.push_back(newAbility.clone());
	return true;
}

bool Player::equipItem(const Item &eqItem)
{
	bool f = false;
	for (size_t i = 0;i < items.size();i++)
	{
		if (items[i]->getID() == eqItem.getID())
		{
			f = true;
			break;
		}
	}
	if (!f) return false;
	if (typeid(eqItem) == typeid(Weapon)) return equipWeapon(eqItem); //CHECK
	switch (((Armor&)eqItem).getArmorType())
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

bool Player::equipAbility(const Ability &eqAbility, int slot)
{
	if (eqAbilities[0].getID() == eqAbility.getID() || eqAbilities[1].getID() == eqAbility.getID() || 
		eqAbilities[2].getID() == eqAbility.getID() || eqAbilities[3].getID() == eqAbility.getID()) return false;
	bool f = false;
	for (size_t i = 0;i < abilities.size();i++)
	{
		if (abilities[i]->getID() == eqAbilities->getID())
		{
			f = true;
			break;
		}
	}
	if (!f) return false;
	eqAbilities[slot] = eqAbility;
	return true;
}

Damage Player::dealDamage(int slot)
{
	int power = eqAbilities[slot].getPower();
	if (power == -1)return { 0,0 };
	return { ((power + 100)*getTotalDamageStats().Physical) / 100, ((power + 100)*getTotalDamageStats().Magical) / 100 };
}

bool Player::levelUp()
{
	if (!Character::levelUp())return false;

	for (size_t i = 0;i < Data::getAbilityCount();i++)
	{
		if (Data::getAbility(i).getMinLevel() == level)
		{
			addAbility(Data::getAbility(i));
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
