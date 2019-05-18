#include "PlayerClasses.h"
#include "Data.h"

Warrior::Warrior(const char *name): Player()
{
	buyItem(Data::getIstance().getItemByID(DEF_WARRIOR_WEAPON_ID), true);
	equipItem(Data::getIstance().getItemByID(DEF_WARRIOR_WEAPON_ID));
	setCharacter(DEF_WARRIOR_STARTER_DAMAGE, DEF_WARRIOR_STARTER_DEFENCE, name, DEF_STARTER_HP, DEF_WARRIOR_STARTER_RES, 1, 0);
	damage += Data::getIstance().getItemByID(DEF_WARRIOR_WEAPON_ID)->getDamageStats();
	defence += Data::getIstance().getItemByID(DEF_STARTER_LEGGINGS_ID)->getDefenceStats();
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
	res.decreaseBy(((2*res.Curr)/res.Max + 1)*DEF_REGEN_RATE);
}

void Warrior::resetRes()
{
	res.Curr = 0;
}

Damage Warrior::dealDamage(int slot)
{
	Damage damage = Player::dealDamage(slot);
	//std::cout << damage.Physical << ' ' << damage.Magical;system("pause");
	if (damage.Physical + damage.Magical <= 0)return { -1,-1 };
	res.increaseBy(eqAbilities[slot]->getResReq() / 2); //could be increased somewhere else as well!
	return { ((100 + res.Curr) * damage.Physical) / 100, damage.Magical };
}

// --------------------------------------------------

Mage::Mage(const char *name) : Player()
{
	buyItem(Data::getIstance().getItemByID(DEF_MAGE_WEAPON_ID), true);
	equipItem(Data::getIstance().getItemByID(DEF_MAGE_WEAPON_ID));
	setCharacter(DEF_MAGE_STARTER_DAMAGE, DEF_MAGE_STARTER_DEFENCE, name, DEF_STARTER_HP, DEF_MAGE_STARTER_RES, 1, 0);
	damage += Data::getIstance().getItemByID(DEF_MAGE_WEAPON_ID)->getDamageStats();
	defence += Data::getIstance().getItemByID(DEF_STARTER_LEGGINGS_ID)->getDefenceStats();
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

void Mage::resetRes()
{
	res.Curr = res.Max;
}

Damage Mage::dealDamage(int slot)
{
	if (eqAbilities[slot]->getResReq() > res.Curr) return { -1,-1 };
	Damage damage = Player::dealDamage(slot);
	if (damage.Physical + damage.Magical <= 0)return { -1,-1 };
	res.decreaseBy(eqAbilities[slot]->getResReq()); //could be increased somewhere else as well!
	return { damage.Physical, 2 * damage.Magical };
}

// --------------------------------------------------

Paladin::Paladin(const char *name)
{
	buyItem(Data::getIstance().getItemByID(DEF_PALADIN_WEAPON_ID), true);
	equipItem(Data::getIstance().getItemByID(DEF_PALADIN_WEAPON_ID));
	setCharacter(DEF_PALADIN_STARTER_DAMAGE, DEF_PALADIN_STARTER_DEFENCE, name, DEF_STARTER_HP, DEF_PALADIN_STARTER_RES, 1, 0);
	damage += Data::getIstance().getItemByID(DEF_PALADIN_WEAPON_ID)->getDamageStats();
	defence += Data::getIstance().getItemByID(DEF_STARTER_LEGGINGS_ID)->getDefenceStats();
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
	if (res.Curr == res.Max)
	{
		res.decreaseBy(res.Max);
		return true;
	}
	else res.increaseBy(DEF_COMBO_INCR_RATE);
	return false;
}

void Paladin::regenRes()
{
	return;
}

void Paladin::resetRes()
{
	res.Curr = 0;
}

Damage Paladin::dealDamage(int slot)
{
	Damage damage = Player::dealDamage(slot);
	if (damage.Physical + damage.Magical <= 0)return { -1,-1 };
	if (gainCombo())
	{
		return { (3 * damage.Physical) / 2, (3 * damage.Magical) / 2 };
	}
	return damage;
}