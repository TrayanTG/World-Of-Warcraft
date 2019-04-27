#pragma once

#include "Data.h"
#include "Bar.h"

class Character
{
protected:
	Damage baseDamage;
	Defence baseDefence;
	Bar HP;
	Bar res;
	char *name;
	int level;
	int XP;
	
public:
	Character(const Damage &baseDamage = { 0,0 }, const Defence &baseDefence = { 0,0,0 }, const char *name = "",
		const Bar &HP = { 0,0 }, const Bar &res = { 0,0 }, int level = 0, int XP = 0);
	Character(const Character&) = delete;
	Character& operator= (const Character &other);
	virtual ~Character();
	
	void setCharacter(const Damage &baseDamage = { 0,0 }, const Defence &baseDefence = { 0,0,0 }, const char *name = "", const Bar &HP = { 0,0 }, const Bar &res = { 0,0 }, int level = 0, int XP = 0);
	const char *getName()const;
	const Bar &getHP()const;
	const Bar &getRes()const;
	int getLevel()const;
	int getXP()const;
	bool levelUp();
	bool gainXP(int XP);
	bool gainDamage(int damage);
	bool isAlive()const;
	
	bool loadCharacter(const char *directory);
	bool saveCharacter(const char *directory);

	virtual int calcDamage(const Damage &damage)const = 0;
	virtual Damage dealDamage(int slot)const = 0;
	virtual Damage getTotalDamageStats()const = 0;
	virtual Defence getTotalDefenceStats()const = 0;
};
