#pragma once
#include "HeadersAndConsts.h"

struct Bar
{
	int Max;
	int Curr;
	Bar(int Max = 0, int Curr = 0) : Max(Max), Curr(Curr) {}
};