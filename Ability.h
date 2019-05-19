#pragma once
#include "HeadersAndConsts.h"
#include "Bar.h"
#include "Box.h"
#include "Damage.h"
#include "Defence.h"


class Ability : public Box
{
	int id;
	Bar cd;
	int minPower;
	int maxPower;
	int minLevel;
	int resReq;
public:
	Ability(int id = -1, int cd = 0, int minPower = 0, int maxPower = 0, int minLevel = 0, int resReq = 0, const string &title = "", 
		const string &description = "", const string &name = "", int size = DEF_ABILITY_SIZE, const COORD &topLeft = { -1,-1 }, bool marked = false);
	Ability(std::ifstream &iFile);

	int getID()const;
	int getPower();
	int getCDRem()const;
	int getCD()const;
	int getMinLevel()const;
	int getResReq()const;
	bool reduceCD(int ms);

	Ability *clone()const;

	virtual void showBox(const Damage &damage)const override;
	void showCD()const;
};