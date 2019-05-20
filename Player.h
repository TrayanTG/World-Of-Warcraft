#pragma once
#include "Character.h"
#include "Weapon.h"
#include "Ability.h"
#include "Armor.h"

class Player : public Character
{
protected:
	std::vector<Item *> items;
	std::vector<Ability *> abilities;
	Armor helmet, shoulders, chest, gloves, legs, feet;
	Weapon weapon;
	Ability eqAbilities[4];

private:
	bool equipHelmet(const Item &eqHelmet);
	bool equipShoulders(const Item &eqShoulders);
	bool equipChest(const Item &eqChest);
	bool equipGloves(const Item &eqGloves);
	bool equipLegs(const Item &eqLegs);
	bool equipFeet(const Item &eqFeet);
	bool equipWeapon(const Item &eqWeapon);

	void init();

public:
	virtual ~Player();

	Player();

	size_t getItemCount()const;
	size_t getAbilityCount()const;

	const Item &getItem(int index)const;
	const Ability &getAbility(int index)const;
	const Ability &getEqAbility(int index)const;
	const Armor &getHelmet()const;
	const Armor &getShoulders()const;
	const Armor &getChest()const;
	const Armor &getGloves()const;
	const Armor &getLegs()const;
	const Armor &getFeet()const;
	const Weapon &getWeapon()const;

	bool reduceCD(int index, int ms);
	bool setItemXY(int index, const Coord &topLeft);
	bool setAbilityXY(int index, const Coord &topLeft);
	bool setEqAbilityXY(int index, const Coord &topLeft);
	bool setHelmetXY(const Coord &topLeft);
	bool setShouldersXY(const Coord &topLeft);
	bool setChestXY(const Coord &topLeft);
	bool setGlovesXY(const Coord &topLeft);
	bool setLegsXY(const Coord &topLeft);
	bool setFeetXY(const Coord &topLeft);
	bool setWeaponXY(const Coord &topLeft);

	bool setItemMarked(int index, bool marked);
	bool setAbilityMarked(int index, bool marked);
	bool setEqAbilityMarked(int index, bool marked);
	bool setHelmetMarked(bool marked);
	bool setShouldersMarked(bool marked);
	bool setChestMarked(bool marked);
	bool setGlovesMarked(bool marked);
	bool setLegsMarked(bool marked);
	bool setFeetMarked(bool marked);
	bool setWeaponMarked(bool marked);

	bool loadPlayer(const string &directory);
	bool savePlayer(const string &directory);

	bool buyItem(const Item &newItem, bool isFree = false);
	bool sellItem(const Item &soldItem);
	bool addAbility(const Ability &newAbility);
	bool equipItem(const Item &eqItem);
	bool equipAbility(const Ability &eqAbility, int slot);

	virtual void regenRes() = 0;
	virtual bool levelUp() override;
	virtual bool gainDamage(const Damage &damage) override;
	virtual int calcDamage(const Damage &damage)const override;
	virtual Damage dealDamage(int slot) override;
	virtual Damage getTotalDamageStats()const override;
	virtual Defence getTotalDefenceStats()const override;
};
