#pragma once
#include "HeadersAndConsts.h"

struct Damage
{
	int Physical;
	int Magical;
	Damage(int Physical = 0, int Magical = 0) : Physical(Physical), Magical(Magical) {}
};