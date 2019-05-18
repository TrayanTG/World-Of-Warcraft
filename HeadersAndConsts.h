#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <exception>
#include <vector>
#include <fstream>
#include <stdio.h> 
#include <time.h>
#include <direct.h>
#include <thread>
#include <utility>
#include <chrono>
#include <conio.h>
#include "Damage.h"
#include "Defence.h"
#include "Bar.h"

//const int KEY_ESC = 27;
const int KEY_ARROW_UP = 38;
const int KEY_ARROW_DOWN = 40;
const int KEY_ARROW_LEFT = 37;
const int KEY_ARROW_RIGHT = 39;


const int MAX_INFOBOX_WIDTH = 16;
const int MAX_PATH_LENGHT = 64;
const int MAX_ID_LENGHT = 8;
const int MAX_WEAPONTYPE_LENGHT = 8;
const int MAX_DESCRIPTION_LENGHT = 256;
const int MAX_LEVEL = 10;
const int MAX_NAME_LENGHT = 16;
const int MAX_ARMORTYPE_LENGHT = 16;
const int MAX_ENEMY_CNT = 16;
const int MAX_LEVEL_DIF = 4;

const int MIN_INFOBOX_WIDTH = MAX_INFOBOX_WIDTH / 2;

const int DEF_CONSOLE_WIDTH = 160;
const int DEF_CONSOLE_HEIGHT = 40;
const int DEF_CONSOLE_SIZE = 15;
const int DEF_INFOBOX_WIDTH = (MAX_INFOBOX_WIDTH + MIN_INFOBOX_WIDTH) / 2;
const int DEF_ENEMY_SIZE = 2;
const int DEF_ITEM_SIZE = 3;
const int DEF_ABILITY_SIZE = 4;
const int DEF_LIST_SIZE = 64;
const int DEF_LEVEL_EXP[MAX_LEVEL - 1] = { 50, 75, 100, 150, 200, 250, 400, 750, 1000 };
const int DEF_FRAME_WIDTH = MAX_NAME_LENGHT + 5; // " (..)"
const int DEF_FRAME_HEIGHT = 4;
const int DEF_CHARACTER_WIDTH = 36;
const int DEF_CHARACTER_HEIGHT = (DEF_CHARACTER_WIDTH * 2) / 3;
const int DEF_CHAR_INFO_WIDTH = 24;
const int DEF_CHAR_INFO_HEIGH = 9;
const int DEF_MAGE_WEAPON_ID = 0;
const int DEF_WARRIOR_WEAPON_ID = 1;
const int DEF_PALADIN_WEAPON_ID = 2;
const int DEF_ABILITY_ID = 0;
const int DEF_ABILITY_SLOT = 0;
const int DEF_REGEN_RATE = 1;
const int DEF_COMBO_INCR_RATE = 1;
const int DEF_STARTER_LEGGINGS_ID = 19;
const int DEF_GOLD_PER_LEVEL = 100;
const int DEF_FREE_BEG = DEF_CHARACTER_WIDTH + DEF_ITEM_SIZE * 2 + 2;
const int DEF_ENEMY_WIDTH = 65;//37;
const int DEF_ENEMY_HEIGHT = 25;


const Damage DEF_WARRIOR_STARTER_DAMAGE = { 10,0 };
const Damage DEF_MAGE_STARTER_DAMAGE = { 0,15 };
const Damage DEF_PALADIN_STARTER_DAMAGE = { 7,7 };
const Defence DEF_WARRIOR_STARTER_DEFENCE = { 3,3,10 };
const Defence DEF_MAGE_STARTER_DEFENCE = { 0,0,0 };
const Defence DEF_PALADIN_STARTER_DEFENCE = { 1,1,5 };
const Damage DEF_WARRIOR_LEVELUP_DAMAGE = { 1,0 };
const Damage DEF_MAGE_LEVELUP_DAMAGE = { 0,2 };
const Damage DEF_PALADIN_LEVELUP_DAMAGE = { 1,1 };
const Defence DEF_WARRIOR_LEVELUP_DEFENCE = { 1,1,5 };
const Defence DEF_MAGE_LEVELUP_DEFENCE = { 0,0,3 };
const Defence DEF_PALADIN_LEVELUP_DEFENCE = { 0,0,5 };
const Bar DEF_STARTER_HP = { 100,100 };
const Bar DEF_WARRIOR_STARTER_RES = { 100,0 };
const Bar DEF_MAGE_STARTER_RES = { 100,100 };
const Bar DEF_PALADIN_STARTER_RES = { 3,0 };

const int ENEMY_WOLF = 0;
const int ENEMY_BEAR = 1;
const int ENEMY_BISON = 2;
const int ENEMY_WITCH = 3;
const int ENEMY_HOGGER = 4;
const int ENEMY_SWINE = 5;
const int ENEMY_FLATTUSK = 6;
const int ENEMY_SCORPION = 7;
const int ENEMY_CRAWLER = 8;
const int ENEMY_GAZZUZ = 9;
const int ENEMY_BOSS_SWINE = 10;
const int ENEMY_BOSS_MURLOC = 11;
const int ENEMY_BOSS_SCORPION = 12;
const int ENEMY_BOSS_CRAWLER = 13;
const int ENEMY_BOSS_HOGGER = 14;

const double DOUBLE_PRECISION = 0.00001;

enum ArmorType {uArmorType, Helmet, Shoulders, Chest, Gloves, Legs, Feet};
enum WeaponType {uWeaponType, Axe, Staff};
enum EnemyType { uEnemyType, Goblin, Dragonkin, DeathKnight }; //P M H
//enum MonsterType {uMonsterType, Rat, Wolf, GiantSpider, Murloc, Witch, Dragon, ElwynnForestBoss};
enum Colour
{
	Black = 0, Blue = 1, Green = 2, Cyan = 3, Red = 4, Magenta = 5, Yellow = 6, LightGray = 7, DarkGray = 8,
	LightBlue = 9, LightGreen = 10, LightCyan = 11, LightRed = 12, LightMagenta = 13, LightYellow = 14, White = 15
};
