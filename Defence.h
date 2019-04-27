#pragma once
#include "HeadersAndConsts.h"

struct Defence
{
	int Armor;
	int MagicResist;
	int Health;
	Defence(int Armor = 0, int MagicResist = 0, int Health = 0) : Armor(Armor), MagicResist(MagicResist), Health(Health) {}
};