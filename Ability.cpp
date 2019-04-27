#include "Ability.h"

Ability::Ability(int id, int cd, int minPower, int maxPower, int minLevel, int resReq, const char *title, const char *description,
	const char *name, int size, bool marked, int tlx, int tly):
	Box(title, description, name, size, marked, tlx, tly)
{
	this->id = id;
	this->cd = cd;
	this->cdRem = 0;
	this->minPower = minPower;
	this->maxPower = maxPower;
	this->minLevel = minLevel;
	this->resReq = resReq;
}

Ability& Ability::operator= (const Ability &other)
{
	if (this == &other) return *this;
	Box::operator=(other);
	id = other.id;
	cd = other.cd;
	cdRem = other.cdRem;
	minPower = other.minPower;
	maxPower = other.maxPower;
	minLevel = other.minLevel;
	resReq = other.resReq;
	return *this;
}

Ability::Ability(std::ifstream &iFile)
{
	int id;
	int cd;
	int minPower;
	int maxPower;
	int minLevel;
	int resReq;
	char title[MAX_INFOBOX_WIDTH - 2];
	char description[MAX_DESCRIPTION_LENGHT];
	char name[DEF_ABILITY_SIZE * 2];
	
	iFile >> id >> cd >> minPower >> maxPower >> minLevel >> resReq >> title;
	iFile.get();
	iFile.getline(description, MAX_DESCRIPTION_LENGHT);
	iFile.getline(name, DEF_ABILITY_SIZE * 2);

	*this = Ability(id, cd, minPower, maxPower, minLevel, resReq, title, description, name, 4);
	
	//*this = Weapon(id, price, minLevel, damage, weaponType, title, description, name, 3);
}

int Ability::getID()const
{
	return id;
}

int Ability::getPower()
{
	if (cdRem > 0)return -1;
	cdRem = cd;
	return minPower + rand()%(maxPower - minPower) + 1;
}

int Ability::getCDRem()const
{
	return cdRem;
}

int Ability::getCD()const
{
	return cd;
}

int Ability::getMinLevel()const
{
	return minLevel;
}

int Ability::getResReq()const
{
	return resReq;
}

bool Ability::reduceCD(int ms)
{
	if (cdRem < 0) { cdRem = 0;return false; }
	cdRem -= ms; 
	if (cdRem < 0) cdRem = 0;
	return true;
}

bool Ability::showBox(const Damage &damage)const
{
	if (Box::showBox() == false)return false;
	int physical = (100 + (minPower + maxPower) / 2) * damage.Physical / 100;
	int magical = (100 + (minPower + maxPower) / 2) * damage.Magical / 100;
	Graphics::getInstance().setcolor(LightRed);
	std::cout << physical;
	if (physical < 10)std::cout << ' ';
	if (physical < 100)std::cout << ' ';
	std::cout << ' ';
	if (magical < 10)std::cout << ' ';
	if (magical < 100)std::cout << ' ';
	Graphics::getInstance().setcolor(LightBlue);
	std::cout << magical;
	Graphics::getInstance().setcolor(White);
	showCD();
	return true;
}

void Ability::showCD()const
{
	int secRem = cdRem / 1000;
	int sec = cd / 1000;
	int dSecRedm = (cdRem % 1000) / 100;
	Graphics::getInstance().gotoxy(tlx + 1, tly + 3);
	if (sec < 10)std::cout << ' ';
	if (secRem < 10)std::cout << ' ';
	std::cout << secRem << '.' << dSecRedm << '/' << sec;
}