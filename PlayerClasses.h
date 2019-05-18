#pragma once
#include "Player.h"

class Warrior: public Player
{

public:
	Warrior(const char *name = "");

	virtual bool levelUp();
	virtual void regenRes();
	virtual void resetRes();
	virtual Damage dealDamage(int slot);
};

class Mage: public Player
{

public:
	Mage(const char *name = "");

	virtual bool levelUp();
	virtual void regenRes();
	virtual void resetRes();
	virtual Damage dealDamage(int slot);
};

class Paladin: public Player
{
public:
	Paladin(const char *name = "");

	bool gainCombo();

	virtual bool levelUp();
	virtual void regenRes();
	virtual void resetRes();
	virtual Damage dealDamage(int slot);
};