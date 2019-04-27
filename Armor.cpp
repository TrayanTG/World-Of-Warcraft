#include "Armor.h"

Armor::Armor(int id, int price, int minLevel, const Defence &defence, ArmorType armorType, const char *title, const char *description, const char *name, int size, bool marked, int tlx, int tly) :
	Item(id, price, minLevel, title, description, name, size, marked, tlx, tly), defence(defence), armorType(armorType)
{

}

Armor::Armor(std::ifstream &iFile)
{
	int id;
	int price;
	int minLevel;
	Defence defence;
	ArmorType armorType;
	char aType[MAX_WEAPONTYPE_LENGHT];
	char title[MAX_INFOBOX_WIDTH - 2];
	char description[MAX_DESCRIPTION_LENGHT];
	char name[DEF_ITEM_SIZE * 2];

	iFile >> id >> price >> minLevel;
	iFile >> defence.Armor >> defence.MagicResist >> defence.Health;
	iFile >> aType;
	if (strcmp(aType, "Helmet")) armorType = Helmet;
	else if (strcmp(aType, "Shoulders")) armorType = Shoulders;
	else if (strcmp(aType, "Chest")) armorType = Chest;
	else if (strcmp(aType, "Gloves")) armorType = Gloves;
	else if (strcmp(aType, "Legs")) armorType = Legs;
	else if (strcmp(aType, "Feet")) armorType = Feet;

	iFile >> title;
	iFile.get();
	iFile.getline(description, MAX_DESCRIPTION_LENGHT);
	iFile.getline(name, DEF_ITEM_SIZE * 2);

	*this = Armor(id, price, minLevel, defence, armorType, title, description, name, 3);
}

bool Armor::showBox()const
{
	if (Box::showBox() == false)return false;
	Graphics::getInstance().setcolor(LightRed);
	std::cout << defence.Armor;
	if (defence.Armor < 10)std::cout << ' ';
	std::cout << ' ';
	if (defence.MagicResist < 10)std::cout << ' ';
	Graphics::getInstance().setcolor(LightBlue);
	std::cout << defence.MagicResist;
	Graphics::getInstance().setcolor(White);
	return true;
}
Damage Armor::getDamage()const
{
	return { 0,0 };
}

Defence Armor::getDefence()const
{
	return defence;
}