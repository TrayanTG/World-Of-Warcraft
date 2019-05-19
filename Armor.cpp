#include "Armor.h"
#include "Graphics.h"

Armor::Armor(int id, int price, int minLevel, const Defence &defence, ArmorType armorType, const string &title, const string &description,
	const string &name, int size, const COORD &topLeft, bool marked):
	Item(id, price, minLevel, title, description, name, size, topLeft, marked), defence(defence), armorType(armorType)
{

}

Armor::Armor(std::ifstream &iFile)
{
	int id;
	int price;
	int minLevel;
	Defence defence;
	ArmorType armorType;
	string aType;
	string title;
	string description;
	string name;

	iFile >> id >> price >> minLevel;
	iFile >> defence.Armor >> defence.MagicResist >> defence.Health;
	iFile >> aType;
	if (aType == "Helmet") armorType = Helmet;
	else if (aType == "Shoulders") armorType = Shoulders;
	else if (aType == "Chest") armorType = Chest;
	else if (aType == "Gloves") armorType = Gloves;
	else if (aType == "Legs") armorType = Legs;
	else if (aType == "Feet") armorType = Feet;
	else armorType = uArmorType;

	iFile >> title;
	iFile.get();
	std::getline(std::cin, description);
	std::getline(std::cin, name);

	*this = Armor(id, price, minLevel, defence, armorType, title, description, name);
}

void Armor::showBox(const Damage &damage)const
{
	if (id >= 0)
	{
		Graphics::getInstance().setcolor(LightRed);
		std::cout << defence.Armor;
		if (defence.Armor < 10)std::cout << ' ';
		std::cout << ' ';
		if (defence.MagicResist < 10)std::cout << ' ';
		Graphics::getInstance().setcolor(LightBlue);
		std::cout << defence.MagicResist;
		Graphics::getInstance().setcolor(White);
	}
	else
	{
		for (int k = 0;k < DEF_ITEM_SIZE - 1;k++)
		{
			Graphics::getInstance().gotoxy(topLeft.X + 1, topLeft.Y + 1 + k);
			for (int i = 0;i < 2 * DEF_ITEM_SIZE - 1;i++)std::cout << 'X';
		}
	}
}
Damage Armor::getDamageStats()const
{
	return { 0,0 };
}

Defence Armor::getDefenceStats()const
{
	return defence;
}

ArmorType Armor::getArmorType()const
{
	return armorType;
}

Item* Armor::clone()const
{
	return new Armor(*this);
}