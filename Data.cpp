#include "Data.h"

Data Data::s;

Data::Data()
{
	items.reserve(DEF_LIST_SIZE);
}

Data::~Data()
{
	for (size_t i = 0;i < enemies.size();i++)delete enemies[i];
	enemies.clear();
	for (size_t i = 0;i < items.size();i++)delete items[i];
	items.clear();
	for (size_t i = 0;i < abilities.size();i++)delete abilities[i];
	abilities.clear();

}

Data& Data::getIstance()
{
	return s;
}

// --------------------------------------------------------------------------------

Item *Data::getItemByID(int id)
{
	for (size_t i = 0;i < items.size();i++)
	{
		if (items[i]->getID() == id)
		{
			return items[i];
		}
	}
	return nullptr;
}

Ability *Data::getAbilityByID(int id)
{
	for (size_t i = 0;i < abilities.size();i++)
	{
		if (abilities[i]->getID() == id)
		{
			return abilities[i];
		}
	}
	return nullptr;
}

Enemy *Data::getEnemyByID(int id)
{
	for (size_t i = 0;i < enemies.size();i++)
	{
		if (enemies[i]->getID() == id)
		{
			return enemies[i];
		}
	}
	return nullptr;
}

void Data::addItem(std::ifstream &iFile)
{
	char type[MAX_NAME_LENGHT];
	iFile >> type;
	Item *temp;
	if (!strcmp(type,"Weapon"))
	{
		//std::cout << "good\n";
		temp = new Weapon(iFile);
		items.push_back(temp);
		//delete[] temp; - dont delete! Using move constructor.....
	}
	else if (!strcmp(type,"Armor"))
	{
		temp = new Armor(iFile);
		items.push_back(temp);
	}
	else { std::cerr << "Invalid item type!\n"; }
}

void Data::loadItems(const char *directory)
{
	char path[MAX_PATH_LENGHT], id[MAX_ID_LENGHT];
	int index = 0, dirLen = strlen(directory);
	strcpy(path, directory);
	std::ifstream iFile(strcat(path, _itoa(index++, id, 10)));
	while(iFile)
	{
		addItem(iFile);
		iFile.close();
		path[dirLen] = 0;
		iFile.open(strcat(path, _itoa(index++, id, 10)));
	}
	iFile.close();
}

void Data::addAbility(std::ifstream &iFile)
{
	Ability *temp;
	temp = new Ability(iFile);
	abilities.push_back(temp);
}

void Data::loadAbilities(const char *directory)
{
	char path[MAX_PATH_LENGHT], id[MAX_ID_LENGHT];
	int index = 0, dirLen = strlen(directory);
	strcpy(path, directory);
	std::ifstream iFile(strcat(path, _itoa(index++, id, 10)));
	while (iFile)
	{
		addAbility(iFile);
		iFile.close();
		path[dirLen] = 0;
		iFile.open(strcat(path, _itoa(index++, id, 10)));
	}
	iFile.close();
}

void Data::addEnemy(std::ifstream &iFile)
{
	Enemy *temp;
	temp = new Enemy(iFile);
	enemies.push_back(temp);
}

void Data::loadEnemies(const char *directory)
{
	char path[MAX_PATH_LENGHT], id[MAX_ID_LENGHT];
	int index = 0, dirLen = strlen(directory);
	strcpy(path, directory);
	std::ifstream iFile(strcat(path, _itoa(index++, id, 10)));
	while (iFile)
	{
		addEnemy(iFile);
		iFile.close();
		path[dirLen] = 0;
		iFile.open(strcat(path, _itoa(index++, id, 10)));
	}
	iFile.close();
}