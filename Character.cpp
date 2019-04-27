#include "Character.h"

void Character::setCharacter(const Damage &baseDamage, const Defence &baseDefence, const char *name, 
	const Bar &HP, const Bar &res, int level, int XP)
{
	this->baseDamage = baseDamage;
	this->baseDefence = baseDefence;
	this->name = new char[strlen(name) + 1];
	strcpy(this->name, name);
	this->HP = HP;
	this->res = res;
	this->level = level;
	this->XP = XP;
}

Character::Character(const Damage &baseDamage, const Defence &baseDefence, const char *name, const Bar &HP, const Bar &res, int level, int XP):
	baseDamage(baseDamage), baseDefence(baseDefence)
{
	this->name = new char[strlen(name) + 1];
	strcpy(this->name, name);
	this->HP = HP;
	this->res = res;
	this->level = level;
	this->XP = XP;
}

Character& Character::operator= (const Character &other)
{
	if (this == &other) return *this;
	baseDamage = other.baseDamage;
	baseDefence = other.baseDefence;
	name = new char[strlen(other.name) + 1];
	strcpy(name, other.name);
	HP = other.HP;
	res = other.res;
	level = other.level;
	XP = other.XP;
	return *this;
}

Character::~Character()
{
	delete[] name;
}

const char *Character::getName()const
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

int Character::getLevel()const
{
	return level;
}

int Character::getXP()const
{
	return XP;
}

bool Character::levelUp()
{
	if (level >= MAX_LEVEL)return false;
	if (XP < DEF_LEVEL_EXP[level - 1])return false;
	XP -= DEF_LEVEL_EXP[level - 1];
	level++;
	baseDamage.Magical++;
	baseDamage.Physical++;
	baseDefence.Armor++;
	baseDefence.MagicResist++;
	baseDefence.Health += 10;
	return true;
}

bool Character::gainXP(int XP)
{
	if (level >= MAX_LEVEL) return false;
	this->XP += XP;
	if (levelUp())return true;
	return false;

}

bool Character::gainDamage(int damage)
{
	HP.Curr -= damage;
	return isAlive();
}

bool Character::isAlive()const
{
	return HP.Curr > 0;
}

bool Character::loadCharacter(const char *directory)
{
	char path[MAX_PATH_LENGHT];
	strcpy(path, directory);
	std::ifstream iFile(strcat(path, "CharacterInfo"));
	if (!iFile) return false;

	char name[MAX_NAME_LENGHT];
	Damage baseDamage;
	Defence baseDefence;
	Bar HP;
	Bar res;
	int level;
	int XP;

	iFile.getline(name, MAX_NAME_LENGHT);
	this->name = new char[strlen(name) + 1];
	strcpy(this->name, name);
	iFile >> baseDamage.Physical >> baseDamage.Magical;
	iFile >> baseDefence.Armor >> baseDefence.MagicResist >> baseDefence.Health;
	iFile >> HP.Max >> res.Max;
	iFile >> level >> XP;
	setCharacter(baseDamage, baseDefence, name, HP, res, level, XP);
	iFile.close();
}

bool Character::saveCharacter(const char *directory)
{
	_mkdir(directory);
	char path[MAX_PATH_LENGHT];
	strcpy(path, directory);
	std::ofstream oFile(strcat(path, "CharacterInfo"));
	if (!oFile) return false;
	oFile << name << '\n';
	oFile << baseDamage.Physical << ' ' << baseDamage.Magical << '\n';
	oFile << baseDefence.Armor << ' ' << baseDefence.MagicResist << ' ' << baseDefence.Health << '\n';
	oFile << HP.Max << ' ' << res.Max << '\n';
	oFile << level << ' ' << XP << '\n';
	oFile.close();
	return true;
}
