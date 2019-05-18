#include "Enemy.h"

Enemy::Enemy(int id, const Damage &damage, const Defence &defence, const Damage &levelUpDamage, const Defence &levelUpDefence,
	const char *name, const Bar &HP, const Bar &res, int level, int XP, int gold):
	Character(damage, defence, name, HP, res, level, XP, gold)
{
	this->id = id;
	this->levelUpDamage = levelUpDamage;
	this->levelUpDefence = levelUpDefence;
	transferDefenceToHP();
	setLevel(level);
}

Enemy::Enemy(const Enemy &other): Character(other)
{
	id = other.id;
	levelUpDamage = other.levelUpDamage;
	levelUpDefence = other.levelUpDefence;
	transferDefenceToHP();
}

Enemy& Enemy::operator= (const Enemy &other)
{
	if (this == &other) return *this;
	Character::operator=(other);
	id = other.id;
	levelUpDamage = other.levelUpDamage;
	levelUpDefence = other.levelUpDefence;
	transferDefenceToHP();
	return *this;
}

Enemy::Enemy(std::ifstream &iFile)
{
	loadEnemy(iFile);
}

Enemy::~Enemy()
{
	//std::cout << "Enemy " << name << " deleted!" << std::endl;
}

void Enemy::setGold()
{
	gold += (damage.Physical + damage.Magical + defence.Armor + defence.MagicResist + HP.Max / 20) / 5;
}

int Enemy::getID()const
{
	return id;
}

void Enemy::setLevel(int level)
{
	for (this->level = 1;this->level < level;this->level++)
	{
		damage += levelUpDamage;
		defence += levelUpDefence;
	}
	transferDefenceToHP();
	setGold();
}

bool Enemy::loadEnemy(std::ifstream &iFile)
{
	if (!iFile) return false;
	iFile >> id;
	iFile >> levelUpDamage.Physical >> levelUpDamage.Magical;
	iFile >> levelUpDefence.Armor >> levelUpDefence.MagicResist >> levelUpDefence.Health;
	iFile.get();
	return loadCharacter(iFile);
}

bool Enemy::saveEnemy(std::ofstream &oFile)
{
	if (!oFile) return false;
	oFile << id << '\n';
	oFile << levelUpDamage.Physical << ' ' << levelUpDamage.Magical << '\n';
	oFile << levelUpDefence.Armor << ' ' << levelUpDefence.MagicResist << ' ' << levelUpDefence.Health << '\n';
	return saveCharacter(oFile);
}

bool Enemy::levelUp()
{
	return false;
}

void Enemy::resetRes()
{
	return;
}

void Enemy::regenRes()
{
	return;
}

bool Enemy::gainDamage(const Damage &damage)
{
	HP.decreaseBy(calcDamage(damage));
	return isAlive();
}

int Enemy::calcDamage(const Damage &damage)const
{
	int physical = damage.Physical - getTotalDefenceStats().Armor;
	int magical = damage.Magical - getTotalDefenceStats().MagicResist;
	if (physical < 0)physical = 0;
	if (magical < 0)magical = 0;
	return physical + magical;
}

Damage Enemy::dealDamage(int slot)
{
	return (getTotalDamageStats()*4)/5 + (getTotalDamageStats() * (rand() % 10)) / 50;
}

Damage Enemy::getTotalDamageStats()const
{
	return damage;
}

Defence Enemy::getTotalDefenceStats()const
{
	return defence;
}
