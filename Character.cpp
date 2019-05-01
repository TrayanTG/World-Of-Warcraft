#include "Character.h"
#include "Data.h"

void Character::setCharacter(const Damage &damage, const Defence &defence, const char *name, 
	const Bar &HP, const Bar &res, int level, int XP, int gold)
{
	this->damage = damage;
	this->defence = defence;
	this->name = new char[strlen(name) + 1];
	strcpy(this->name, name);
	this->HP = HP;
	this->res = res;
	this->level = level;
	this->XP = XP;
	this->gold = gold;
}

Character::Character(const Damage &damage, const Defence &defence, const char *name, const Bar &HP, const Bar &res, int level, int XP, int gold):
	damage(damage), defence(defence)
{
	this->name = new char[strlen(name) + 1];
	strcpy(this->name, name);
	this->HP = HP;
	this->res = res;
	this->level = level;
	this->XP = XP;
	this->gold = gold;
	transferDefenceToHP();
}

Character& Character::operator= (const Character &other)
{
	if (this == &other) return *this;
	damage = other.damage;
	defence = other.defence;
	name = new char[strlen(other.name) + 1];
	strcpy(name, other.name);
	HP = other.HP;
	res = other.res;
	level = other.level;
	XP = other.XP;
	gold = other.gold;
	transferDefenceToHP();
	return *this;
}

Character::Character(std::ifstream &iFile)
{
	char name[MAX_NAME_LENGHT];
	Damage damage;
	Defence defence;
	Bar HP;
	Bar res;
	int level;
	int XP;
	int gold;

	//std::cin.get(); PROBABLY
	iFile.getline(name, MAX_NAME_LENGHT);
	this->name = new char[strlen(name) + 1];
	strcpy(this->name, name);
	iFile >> damage.Physical >> damage.Magical;
	iFile >> defence.Armor >> defence.MagicResist >> defence.Health;
	iFile >> HP.Max >> HP.Curr;
	iFile >> res.Max >> res.Curr;
	iFile >> level >> XP >> gold;
	setCharacter(damage, defence, name, HP, res, level, XP, gold);
}

Character::~Character()
{
	//std::cout << name << " deleted!" << std::endl;
	delete[] name;
}

void Character::transferDefenceToHP()
{
	HP.Max += defence.Health;
	HP.Curr = HP.Max;
	defence.Health = 0;
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

bool Character::gainDamage(int damage)
{
	HP.decreaseBy(damage);
	return isAlive();
}

void Character::gainGold(int gold)
{
	this->gold += gold;
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
	int gold;

	iFile.getline(name, MAX_NAME_LENGHT);
	this->name = new char[strlen(name) + 1];
	strcpy(this->name, name);
	iFile >> baseDamage.Physical >> baseDamage.Magical;
	iFile >> baseDefence.Armor >> baseDefence.MagicResist >> baseDefence.Health;
	iFile >> HP.Max >> HP.Curr;
	iFile >> res.Max >> res.Curr;
	iFile >> level >> XP >> gold;
	setCharacter(baseDamage, baseDefence, name, HP, res, level, XP, gold);
	iFile.close();
	return true;
}

bool Character::saveCharacter(const char *directory)
{
	_mkdir(directory);
	char path[MAX_PATH_LENGHT];
	strcpy(path, directory);
	std::ofstream oFile(strcat(path, "CharacterInfo"));
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
	char name[MAX_NAME_LENGHT];
	Damage baseDamage;
	Defence baseDefence;
	Bar HP;
	Bar res;
	int level;
	int XP;
	int gold;

	//std::cin.get(); PROBABLY
	iFile.getline(name, MAX_NAME_LENGHT);
	this->name = new char[strlen(name) + 1];
	strcpy(this->name, name);
	iFile >> baseDamage.Physical >> baseDamage.Magical;
	iFile >> baseDefence.Armor >> baseDefence.MagicResist >> baseDefence.Health;
	iFile >> HP.Max >> HP.Curr;
	iFile >> res.Max >> res.Curr;
	iFile >> level >> XP >> gold;
	setCharacter(baseDamage, baseDefence, name, HP, res, level, XP, gold);
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
