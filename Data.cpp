#include "Data.h"

Data Data::s;

Data::Data()
{
	items.reserve(DEF_LIST_SIZE);
}

Data::~Data()
{

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

void Data::addItem(std::ifstream &iFile)
{
	int t;iFile >> t;
	Item *temp;
	if (t == 0)
	{
		//std::cout << "good\n";
		temp = new Weapon(iFile);
		items.push_back(temp);
		//delete[] temp; - dont delete! Using move constructor.....
	}
	else if (t == 1)
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