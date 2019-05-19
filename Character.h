#pragma once
#include "Bar.h"
#include "Damage.h"
#include "Defence.h"
#include "HeadersAndConsts.h"

class Character
{
protected:
	string	name;
	Damage	damage;
	Defence	defence;
	Bar		HP;
	Bar		res;
	int		level;
	int		XP;
	int		gold;
	
	void transferDefenceToHP();

public:
	Character(const Damage &damage = { 0,0 }, const Defence &defence = { 0,0,0 }, const string &name = "",
		const Bar &HP = { 0,0 }, const Bar &res = { 0,0 }, int level = 1, int XP = 0, int gold = 0);
	Character(std::ifstream &iFile);
	virtual ~Character();
	
	void setCharacter(const Damage &damage = { 0,0 }, const Defence &defence = { 0,0,0 }, const string &name = "", const Bar &HP = { 0,0 }, const Bar &res = { 0,0 }, int level = 0, int XP = 0, int gold = 0); //For PlayerClasses.h
	const string &getName()const;
	const Bar &getRes()const;
	const Bar &getHP()const;
	bool isAlive()const;
	int getLevel()const;
	int getGold()const;
	int getXP()const;

	void gainGold(int gold);
	bool gainXP(int XP);
	void setMaxHP();
	
	bool loadCharacter(const string &directory);
	bool saveCharacter(const string &directory);
	bool loadCharacter(std::ifstream &iFile);
	bool saveCharacter(std::ofstream &oFile);

	virtual bool levelUp();
	virtual void resetRes() = 0;
	virtual void regenRes() = 0;
	virtual Damage dealDamage(int slot) = 0;
	virtual Damage getTotalDamageStats()const = 0;
	virtual Defence getTotalDefenceStats()const = 0;
	virtual bool gainDamage(const Damage &damage) = 0;
	virtual int calcDamage(const Damage &damage)const = 0;
};
