#pragma once
#include "HeadersAndConsts.h"
#include "Graphics.h"
#include "Box.h"
#include "Damage.h"
#include "Defence.h"


class Ability : public Box
{
	int id;
	int cd;
	int cdRem;
	int minPower;
	int maxPower;
	int minLevel;
	int resReq;
public:
	Ability(int id = 0, int cd = 0, int minPower = 0, int maxPower = 0, int minLevel = 0, int resReq = 0, const char *title = "", 
		const char *description = "", const char *name = "", int size = 0, bool marked = false, int tlx = -1, int tly = -1);
	Ability(const Ability&) = delete;
	Ability& operator= (const Ability &other);
	Ability(std::ifstream &iFile);

	int getID()const;
	int getPower();
	int getCDRem()const;
	int getCD()const;
	int getMinLevel()const;
	int getResReq()const;
	bool reduceCD(int ms);

	bool showBox(const Damage &damage)const;
	void showCD()const;
};