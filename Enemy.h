#pragma once
#include "Character.h"

class Enemy: public Character
{
	int id;
	Damage levelUpDamage;
	Defence levelUpDefence;

	void setGold();

public:
	Enemy(int id = -1, const Damage &damage = { 0,0 }, const Defence &defence = { 0,0,0 }, 
		const Damage &levelUpDamage = { 0,0 }, const Defence &levelUpDefence = { 0,0 }, const string &name = "",
		const Bar &HP = { 0,0 }, const Bar &res = { 0,0 }, int level = 1, int XP = 0, int gold = 0);
	Enemy(std::ifstream &iFile);

	int getID()const;
	void setLevel(int level);

	bool loadEnemy(std::ifstream &iFile);
	bool saveEnemy(std::ofstream &oFile);

	virtual bool levelUp() override;
	virtual void resetRes() override;
	virtual void regenRes() override;
	virtual Damage dealDamage(int slot = 0) override;
	virtual Damage getTotalDamageStats()const override;
	virtual Defence getTotalDefenceStats()const override;
	virtual bool gainDamage(const Damage &damage) override;
	virtual int calcDamage(const Damage &damage)const override;
};
