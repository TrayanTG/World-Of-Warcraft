#include "Weapon.h"
#include "Graphics.h"

Weapon::Weapon(int id, int price, int minLevel, const Damage &damage, WeaponType weaponType, const char *title, 
	const char *description, const char *name, int size, bool marked, int tlx, int tly) :
	Item(id, price, minLevel, title, description, name, size, marked, tlx, tly), damage(damage), weaponType(weaponType)
{

}

Weapon::Weapon(std::ifstream &iFile)
{
	int id;
	int price;
	int minLevel;
	Damage damage;
	WeaponType weaponType;
	char wType[MAX_WEAPONTYPE_LENGHT];
	char title[MAX_INFOBOX_WIDTH - 2];
	char description[MAX_DESCRIPTION_LENGHT];
	char name[DEF_ITEM_SIZE*2];

	iFile >> id >> price >> minLevel;
	iFile >> damage.Physical >> damage.Magical;
	iFile >> wType;
	if (!strcmp(wType, "Axe")) weaponType = Axe;
	else if (!strcmp(wType, "Staff")) weaponType = Staff;
	else weaponType = uWeaponType;
	iFile >> title;
	iFile.get();
	iFile.getline(description, MAX_DESCRIPTION_LENGHT);
	iFile.getline(name, DEF_ITEM_SIZE*2);

	*this = Weapon(id, price, minLevel, damage, weaponType, title, description, name, 3);
}

bool Weapon::showBox()const
{
	if (Box::showBox() == false)return false;
	Graphics::getInstance().setcolor(LightRed);
	std::cout << damage.Physical;
	if (damage.Physical < 10)std::cout << ' ';
	std::cout << ' ';
	if (damage.Magical < 10)std::cout << ' ';
	Graphics::getInstance().setcolor(LightBlue);
	std::cout << damage.Magical;
	Graphics::getInstance().setcolor(White);
	return true;
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