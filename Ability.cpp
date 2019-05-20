#include "Ability.h"
#include "Graphics.h"

Ability::Ability(int id, int cd, int minPower, int maxPower, int minLevel, int resReq, const string &title, const string &description, 
	const string &name, int size, const Coord &topLeft, bool marked):
	Box(title, description, name, size, topLeft, marked), cd(cd, 0)
{
	this->id = id;
	this->minPower = minPower;
	this->maxPower = maxPower;
	this->minLevel = minLevel;
	this->resReq = resReq;
}

Ability::Ability(std::ifstream &iFile)
{
	int id;
	int cd;
	int minPower;
	int maxPower;
	int minLevel;
	int resReq;
	string title;
	string description;
	string name;
	
	iFile >> id >> cd >> minPower >> maxPower >> minLevel >> resReq >> title;
	iFile.get();
	std::getline(iFile, description);
	std::getline(iFile, name);

	*this = Ability(id, cd, minPower, maxPower, minLevel, resReq, title, description, name);
}

int Ability::getID()const
{
	return id;
}

int Ability::getPower()
{
	if (cd.Curr > 0)return -1000;
	cd.increaseBy(cd.Max);
	return minPower + rand()%(maxPower - minPower + 1) + 1;
}

int Ability::getCDRem()const
{
	return cd.Curr;
}

int Ability::getCD()const
{
	return cd.Max;
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
	if (cd.Curr < 0) { cd.Curr = 0;return false; }
	cd.Curr -= ms; 
	if (cd.Curr < 0) cd.Curr = 0;
	return true;
}

Ability *Ability::clone()const
{
	return new Ability(*this);
}

void Ability::showBox(const Damage &damage)const
{
	Box::showBox(damage);
	if (id >= 0)
	{
		int physical = (100 + (minPower + maxPower) / 2) * damage.Physical / 100;
		int magical = (100 + (minPower + maxPower) / 2) * damage.Magical / 100;
		Graphics::setcolor(LightRed);
		std::cout << physical;
		if (physical < 10)std::cout << ' ';
		if (physical < 100)std::cout << ' ';
		std::cout << ' ';
		if (magical < 10)std::cout << ' ';
		if (magical < 100)std::cout << ' ';
		Graphics::setcolor(LightBlue);
		std::cout << magical;
		Graphics::setcolor(White);
		showCD();
	}
	else
	{
		for (int k = 0;k < DEF_ABILITY_SIZE - 1;k++)
		{
			Graphics::gotoxy(topLeft.X + 1, topLeft.Y + 1 + k);
			for (int i = 0;i < 2 * DEF_ABILITY_SIZE - 1;i++)std::cout << 'X';
		}
	}
}

void Ability::showCD()const
{
	int secRem = cd.Curr / 1000;
	int sec = cd.Max / 1000;
	int dSecRedm = (cd.Curr % 1000) / 100;
	Graphics::gotoxy(topLeft.X + 1, topLeft.Y + 3);
	if (sec < 10)std::cout << ' ';
	if (secRem < 10)std::cout << ' ';
	std::cout << secRem << '.' << dSecRedm << '/' << sec;
}
