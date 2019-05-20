#pragma once
#include "Player.h"

class Warrior: public Player
{

public:
	Warrior(const string &name = "");

	virtual bool levelUp() override;
	virtual void regenRes() override;
	virtual void resetRes() override;
	virtual Damage dealDamage(int slot) override;
};

class Mage: public Player
{

public:
	Mage(const string &name = "");

	virtual bool levelUp() override;
	virtual void regenRes() override;
	virtual void resetRes() override;
	virtual Damage dealDamage(int slot) override;
};

class Paladin: public Player
{
public:
	Paladin(const string &name = "");

	bool gainCombo();

	virtual bool levelUp() override;
	virtual void regenRes() override;
	virtual void resetRes() override;
	virtual Damage dealDamage(int slot) override;
};