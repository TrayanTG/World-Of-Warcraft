#include "Data.h"

std::vector<Item*> Data::items;
std::vector<Ability*> Data::abilities;
std::vector<Enemy*> Data::enemies;

const Weapon Data::emptyWeapon;
const Armor Data::emptyArmor;
const Ability Data::emptyAbility;

Data::Data()
{
	
}

Data::~Data()
{
	for (size_t i = 0;i < enemies.size();i++)delete enemies[i];
	for (size_t i = 0;i < items.size();i++)delete items[i];
	for (size_t i = 0;i < abilities.size();i++)delete abilities[i];
}

// --------------------------------------------------------------------------------

size_t Data::getItemCount()
{
	return items.size();
}

size_t Data::getAbilityCount()
{
	return abilities.size();
}

size_t Data::getEnemyCount()
{
	return enemies.size();
}

const Item &Data::getItem(int index)
{
	if ((size_t)index >= items.size())throw std::runtime_error("const Item &getItem(int) overflow!\n");
	return *items[index];
}

const Ability &Data::getAbility(int index)
{
	if ((size_t)index >= abilities.size())throw std::runtime_error("const Ability &getAbility(int) overflow!\n");
	return *abilities[index];
}

const Enemy &Data::getEnemy(int index)
{
	if ((size_t)index >= enemies.size())throw std::runtime_error("const Enemy &getEnemy(int) overflow!\n");
	return *enemies[index];
}

const Item &Data::getItemByID(int id)
{
	for (size_t i = 0;i < items.size();i++)
	{
		if (items[i]->getID() == id)
		{
			return *items[i];
		}
	}
	throw std::runtime_error("const Item &getItemByID(int) bad call!\n");
}

const Ability &Data::getAbilityByID(int id)
{
	for (size_t i = 0;i < abilities.size();i++)
	{
		if (abilities[i]->getID() == id)
		{
			return *abilities[i];
		}
	}
	throw std::runtime_error("const Ability &getAbilityByID(int) bad call!\n");
}

const Enemy &Data::getEnemyByID(int id)
{
	for (size_t i = 0;i < enemies.size();i++)
	{
		if (enemies[i]->getID() == id)
		{
			return *enemies[i];
		}
	}
	throw std::runtime_error("const Enemy &getEnemyByID(int) bad call!\n");
}

const Armor &Data::getEmptyArmor()
{
	return emptyArmor;
}

const Weapon &Data::getEmptyWeapon()
{
	return emptyWeapon;
}

const Ability &Data::getEmptyAbility()
{
	return emptyAbility;
}

void Data::addItem(std::ifstream &iFile)
{
	string type;
	iFile >> type;
	Item *temp;
	if (type == "Weapon")
	{
		temp = new Weapon(iFile);
		items.push_back(temp);
		}
	else if (type == "Armor")
	{
		temp = new Armor(iFile);
		items.push_back(temp);
	}
	else { std::cerr << "Invalid item type!\n"; }
	// Don't delete *temp - using move constuctor!
}

void Data::loadItems(const string &directory)
{
	char id[MAX_ID_LENGHT];
	int index = 0;
	std::ifstream iFile(directory + _itoa(index++, id, 10));
	while(iFile)
	{
		addItem(iFile);
		iFile.close();
		iFile.open(directory + _itoa(index++, id, 10));
	}
	iFile.close();
}

void Data::addAbility(std::ifstream &iFile)
{
	Ability *temp;
	temp = new Ability(iFile);
	abilities.push_back(temp);
}

void Data::loadAbilities(const string &directory)
{
	char id[MAX_ID_LENGHT];
	int index = 0;
	std::ifstream iFile(directory + _itoa(index++, id, 10));
	while (iFile)
	{
		addAbility(iFile);
		iFile.close();
		iFile.open(directory + _itoa(index++, id, 10));
	}
	iFile.close();
}

void Data::addEnemy(std::ifstream &iFile)
{
	Enemy *temp;
	temp = new Enemy(iFile);
	enemies.push_back(temp);
}

void Data::loadEnemies(const string &directory)
{
	char id[MAX_ID_LENGHT];
	int index = 0;
	std::ifstream iFile(directory + _itoa(index++, id, 10));
	while (iFile)
	{
		addEnemy(iFile);
		iFile.close();
		iFile.open(directory + _itoa(index++, id, 10));
	}
	iFile.close();
}
