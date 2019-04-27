#pragma once
#include "Data.h"

class Character
{
protected:
	char *name;
	int maxHP;
	int currHP;
	int level;
	
public:
	Character(const char *name = "", int maxHP = 0, int level = 0);
	Character(const Character&) = delete;
	Character& operator= (const Character &other);
	virtual ~Character();

	const char *getName()const;
	int getCurrHP()const;
	int getMaxHP()const;
	int getLevel()const;
	
	virtual Damage getDamage()const = 0;
	virtual Defence getDefence()const = 0;
};