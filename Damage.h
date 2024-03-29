#pragma once

struct Damage
{
	int Physical;
	int Magical;
	Damage(int Physical = 0, int Magical = 0) : Physical(Physical), Magical(Magical) {}
	Damage& operator += (const Damage &other)
	{
		Physical += other.Physical;
		Magical += other.Magical;
		return *this;
	}
	Damage operator + (const Damage &other)
	{
		return { Physical + other.Physical, Magical + other.Magical };
	}
	Damage operator * (int mult)
	{
		return { Physical * mult, Magical * mult };
	}
	Damage& operator -= (const Damage &other)
	{
		Physical -= other.Physical;
		Magical -= other.Magical;
		return *this;
	}
	Damage operator / (int mult)
	{
		return { Physical / mult, Magical / mult };
	}
};