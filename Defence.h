#pragma once
#include "HeadersAndConsts.h"

struct Defence
{
	int Armor;
	int MagicResist;
	int Health;
	Defence(int Armor = 0, int MagicResist = 0, int Health = 0) : Armor(Armor), MagicResist(MagicResist), Health(Health) {}
	Defence& operator += (const Defence &other)
	{
		Armor += other.Armor;
		MagicResist += other.MagicResist;
		Health += other.Health;
		return *this;
	}
};