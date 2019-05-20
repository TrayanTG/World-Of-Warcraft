#include "Character.h"
#include "Data.h"

void Character::setCharacter(const Damage &damage, const Defence &defence, const string &name, 
	const Bar &HP, const Bar &res, int level, int XP, int gold)
{
	this->damage = damage;
	this->defence = defence;
	this->name = name;
	this->HP = HP;
	this->res = res;
	this->level = level;
	this->XP = XP;
	this->gold = gold;
	transferDefenceToHP();
}

Character::Character(const Damage &damage, const Defence &defence, const string &name, const Bar &HP, const Bar &res, int level, int XP, int gold):
	damage(damage), defence(defence), name(name), HP(HP), res(res)
{
	this->level = level;
	this->XP = XP;
	this->gold = gold;
	transferDefenceToHP();
}

Character::Character(std::ifstream &iFile)
{
	loadCharacter(iFile);
}

Character::~Character()
{
	
}

void Character::transferDefenceToHP()
{
	HP.Max += defence.Health;
	HP.Curr = HP.Max;
	defence.Health = 0;
}

const string &Character::getName()const
{
	return name;
}

const Bar &Character::getHP()const
{
	return HP;
}

const Bar &Character::getRes()const
{
	return res;
}

void Character::setMaxHP()
{
	HP.Curr = HP.Max;
}

int Character::getLevel()const
{
	return level;
}

int Character::getXP()const
{
	return XP;
}

int Character::getGold()const
{
	return gold;
}

bool Character::levelUp()
{
	if (level >= MAX_LEVEL)return false;
	if (XP < DEF_LEVEL_EXP[level - 1])return false;
	XP -= DEF_LEVEL_EXP[level - 1];
	level++;
	gold += level * DEF_GOLD_PER_LEVEL;
	return true;
}

bool Character::gainXP(int XP)
{
	if (level >= MAX_LEVEL) return false;
	this->XP += XP;
	return levelUp();
}

void Character::gainGold(int gold)
{
	this->gold += gold;
}

bool Character::isAlive()const
{
	return HP.Curr > 0;
}

bool Character::loadCharacter(const string &directory)
{
	string path;
	path = directory;
	std::ifstream iFile(path + "CharacterInfo");
	if (!iFile) return false;

	getline(iFile, name);
	iFile >> damage.Physical >> damage.Magical;
	iFile >> defence.Armor >> defence.MagicResist >> defence.Health;
	iFile >> HP.Max >> HP.Curr;
	iFile >> res.Max >> res.Curr;
	iFile >> level >> XP >> gold;
	iFile.close();
	return true;
}

bool Character::saveCharacter(const string &directory)
{
	_mkdir(directory.c_str());
	string path = directory;
	std::ofstream oFile(path + "CharacterInfo");
	if (!oFile) return false;
	oFile << name << '\n';
	oFile << damage.Physical << ' ' << damage.Magical << '\n';
	oFile << defence.Armor << ' ' << defence.MagicResist << ' ' << defence.Health << '\n';
	oFile << HP.Max << ' ' << HP.Curr << '\n';
	oFile << res.Max << ' ' << res.Curr << '\n';
	oFile << level << ' ' << XP << ' ' << gold << '\n';
	oFile.close();
	return true;
}

bool Character::loadCharacter(std::ifstream &iFile)
{
	if (!iFile) return false;
	//std::cin.get(); PROBABLY
	std::getline(iFile, name);
	iFile >> damage.Physical >> damage.Magical;
	iFile >> defence.Armor >> defence.MagicResist >> defence.Health;
	iFile >> HP.Max >> HP.Curr;
	iFile >> res.Max >> res.Curr;
	iFile >> level >> XP >> gold;
	return true;
}

bool Character::saveCharacter(std::ofstream &oFile)
{
	if (!oFile) return false;
	oFile << name << '\n';
	oFile << damage.Physical << ' ' << damage.Magical << '\n';
	oFile << defence.Armor << ' ' << defence.MagicResist << ' ' << defence.Health << '\n';
	oFile << HP.Max << ' ' << HP.Curr << '\n';
	oFile << res.Max << ' ' << res.Curr << '\n';
	oFile << level << ' ' << XP << ' ' << gold << '\n';
	return true;
}
