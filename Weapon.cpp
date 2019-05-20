#include "Weapon.h"
#include "Graphics.h"

Weapon::Weapon(int id, int price, int minLevel, const Damage &damage, WeaponType weaponType, const string &title, const string &description, 
	const string &name, int size, const Coord &topLeft, bool marked):
	Item(id, price, minLevel, title, description, name, size, topLeft, marked), damage(damage), weaponType(weaponType)
{
	
}

Weapon::Weapon(std::ifstream &iFile)
{
	int id;
	int price;
	int minLevel;
	Damage damage;
	WeaponType weaponType;
	string wType;
	string title;
	string description;
	string name;

	iFile >> id >> price >> minLevel;
	iFile >> damage.Physical >> damage.Magical;
	iFile >> wType;
	if (wType == "Axe") weaponType = Axe;
	else if (wType == "Staff") weaponType = Staff;
	else weaponType = uWeaponType;
	iFile >> title;
	iFile.get();
	std::getline(iFile, description);
	std::getline(iFile, name);

	*this = Weapon(id, price, minLevel, damage, weaponType, title, description, name);
}

void Weapon::showBox(const Damage &damage)const
{
	Box::showBox();
	if (id >= 0)
	{
		Graphics::setcolor(LightRed);
		std::cout << this->damage.Physical;
		if (this->damage.Physical < 10)std::cout << ' ';
		std::cout << ' ';
		if (this->damage.Magical < 10)std::cout << ' ';
		Graphics::setcolor(LightBlue);
		std::cout << this->damage.Magical;
		Graphics::setcolor(White);
	}
	else
	{
		for (int k = 0;k < DEF_ITEM_SIZE - 1;k++)
		{
			Graphics::gotoxy(topLeft.X + 1, topLeft.Y + 1 + k);
			for (int i = 0;i < 2 * DEF_ITEM_SIZE - 1;i++)std::cout << 'X';
		}
	}
}

Damage Weapon::getDamageStats()const
{
	return damage;
}

Defence Weapon::getDefenceStats()const
{
	return { 0,0 };
}

WeaponType Weapon::getWeaponType()const
{
	return weaponType;
}

Item* Weapon::clone()const
{
	return new Weapon(*this);
}
