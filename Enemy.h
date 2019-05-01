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
		const Damage &levelUpDamage = { 0,0 }, const Defence &levelUpDefence = { 0,0 }, const char *name = "",
		const Bar &HP = { 0,0 }, const Bar &res = { 0,0 }, int level = 1, int XP = 0, int gold = 0);
	Enemy(const Enemy &other) = delete;
	Enemy(Enemy &&other) = delete;
	Enemy& operator= (const Enemy &other);
	Enemy(std::ifstream &iFile);
	~Enemy();

	int getID();
	void setLevel(int level);

	bool loadEnemy(std::ifstream &iFile);
	bool saveEnemy(std::ofstream &oFile);

	virtual bool levelUp();
	virtual void regenRes();
	virtual int calcDamage(const Damage &damage)const;
	virtual Damage dealDamage(int slot)const;
	virtual Damage getTotalDamageStats()const;
	virtual Defence getTotalDefenceStats()const;
};
