#include "Character.h"

Character::Character(const char *name, int maxHP, int level)
{
	this->name = new char[strlen(name) + 1];
	strcpy(this->name, name);
	this->currHP = maxHP;
	this->maxHP = maxHP;
	this->level = level;
}

Character& Character::operator= (const Character &other)
{
	if (this == &other) return *this;
	name = new char[strlen(other.name) + 1];
	strcpy(name, other.name);
	currHP = other.currHP;
	maxHP = other.maxHP;
	level = other.level;
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

int Character::getCurrHP()const
{
	return currHP;
}

int Character::getMaxHP()const
{
	return maxHP;
}

int Character::getLevel()const
{
	return level;
}