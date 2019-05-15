#pragma once
#include "Bar.h"
#include "Damage.h"
#include "Defence.h"
#include "HeadersAndConsts.h"

class Character
{
protected:
	Damage damage;
	Defence defence;
	Bar HP;
	Bar res;
	char *name;
	int level;
	int XP;
	int gold;
	
	void transferDefenceToHP();

public:
	Character(const Damage &damage = { 0,0 }, const Defence &defence = { 0,0,0 }, const char *name = "",
		const Bar &HP = { 0,0 }, const Bar &res = { 0,0 }, int level = 1, int XP = 0, int gold = 0);
	Character(const Character &other);
	Character& operator= (const Character &other);
	Character(std::ifstream &iFile);
	virtual ~Character();
	
	void setCharacter(const Damage &damage = { 0,0 }, const Defence &defence = { 0,0,0 }, const char *name = "", const Bar &HP = { 0,0 }, const Bar &res = { 0,0 }, int level = 0, int XP = 0, int gold = 0);
	const char *getName()const;
	const Bar &getHP()const;
	const Bar &getRes()const;
	int getLevel()const;
	int getXP()const;
	int getGold()const;
	bool gainXP(int XP);
	void gainGold(int gold);
	bool gainDamage(int damage);
	bool isAlive()const;
	
	bool loadCharacter(const char *directory);
	bool saveCharacter(const char *directory);
	bool loadCharacter(std::ifstream &iFile);
	bool saveCharacter(std::ofstream &oFile);

	virtual bool levelUp();
	virtual void regenRes() = 0;
	virtual int calcDamage(const Damage &damage)const = 0;
	virtual Damage dealDamage(int slot)const = 0;
	virtual Damage getTotalDamageStats()const = 0;
	virtual Defence getTotalDefenceStats()const = 0;
};
