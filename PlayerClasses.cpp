#include "PlayerClasses.h"
#include "Data.h"

Warrior::Warrior(const char *name): Player()
{
	buyItem(Data::getIstance().getItemByID(DEF_WARRIOR_WEAPON_ID), true);
	equipItem(Data::getIstance().getItemByID(DEF_WARRIOR_WEAPON_ID));
	setCharacter(DEF_WARRIOR_STARTER_DAMAGE, DEF_WARRIOR_STARTER_DEFENCE, name, DEF_STARTER_HP, DEF_WARRIOR_STARTER_RES, 1, 0);
}

bool Warrior::levelUp()
{
	if (!Player::levelUp())return false;
	damage += DEF_WARRIOR_LEVELUP_DAMAGE;
	defence += DEF_WARRIOR_LEVELUP_DEFENCE;
	transferDefenceToHP();
	return true;
}

void Warrior::regenRes()
{
	res.decreaseBy(DEF_REGEN_RATE);
}

Damage Warrior::dealDamage(int slot)
{
	return { ((100 + res.Curr)*Player::dealDamage(slot).Physical) / 100,((100 + res.Curr)*Player::dealDamage(slot).Magical) / 100 };
}

// --------------------------------------------------

Mage::Mage(const char *name) : Player()
{
	buyItem(Data::getIstance().getItemByID(DEF_MAGE_WEAPON_ID), true);
	equipItem(Data::getIstance().getItemByID(DEF_MAGE_WEAPON_ID));
	setCharacter(DEF_MAGE_STARTER_DAMAGE, DEF_MAGE_STARTER_DEFENCE, name, DEF_STARTER_HP, DEF_MAGE_STARTER_RES, 1, 0);
}

bool Mage::levelUp()
{
	if (!Player::levelUp())return false;
	damage += DEF_MAGE_LEVELUP_DAMAGE;
	defence += DEF_MAGE_LEVELUP_DEFENCE;
	transferDefenceToHP();
	return true;
}

void Mage::regenRes()
{
	res.increaseBy(DEF_REGEN_RATE);
}

Damage Mage::dealDamage(int slot)
{
	return Player::dealDamage(slot);
}

// --------------------------------------------------

Paladin::Paladin(const char *name)
{
	buyItem(Data::getIstance().getItemByID(DEF_PALADIN_WEAPON_ID), true);
	equipItem(Data::getIstance().getItemByID(DEF_PALADIN_WEAPON_ID));
	setCharacter(DEF_PALADIN_STARTER_DAMAGE, DEF_PALADIN_STARTER_DEFENCE, name, DEF_STARTER_HP, DEF_PALADIN_STARTER_RES, 1, 0);
}

bool Paladin::levelUp()
{
	if (!Player::levelUp())return false;
	damage += DEF_PALADIN_LEVELUP_DAMAGE;
	defence += DEF_PALADIN_LEVELUP_DEFENCE;
	transferDefenceToHP();
	return true;
}

bool Paladin::gainCombo()
{
	res.increaseBy(DEF_COMBO_INCR_RATE);
	if (res.Curr >= res.Max)
	{
		res.decreaseBy(res.Max);
		return true;
	}
	return false;
}

void Paladin::regenRes()
{

}

Damage Paladin::dealDamage(int slot)
{
	Damage temp = Player::dealDamage(slot);
	if (temp.Physical + temp.Magical <= 0)return { 0,0 };
	if (gainCombo())
	{
		return { 2 * temp.Physical, 2 * temp.Magical };
	}
	return temp;
}