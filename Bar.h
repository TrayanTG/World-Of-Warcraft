#pragma once

struct Bar
{
	int Max;
	int Curr;
	Bar(int Max = 0, int Curr = 0) : Max(Max), Curr(Curr) {}
	void increaseBy(int amount) { Curr += amount; if (Curr > Max)Curr = Max; if (Curr < 0)Curr = 0; }
	void decreaseBy(int amount) { increaseBy(-amount); }
};