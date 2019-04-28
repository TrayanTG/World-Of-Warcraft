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

const int MAX_INFOBOX_WIDTH = 16;
const int MAX_PATH_LENGHT = 64;
const int MAX_ID_LENGHT = 8;
const int MAX_WEAPONTYPE_LENGHT = 8;
const int MAX_DESCRIPTION_LENGHT = 256;
const int MAX_LEVEL = 10;
const int MAX_NAME_LENGHT = 16;
const int MAX_ARMORTYPE_LENGHT = 16;

const int MIN_INFOBOX_WIDTH = MAX_INFOBOX_WIDTH / 2;

const int DEF_CONSOLE_WIDTH = 160;
const int DEF_CONSOLE_HEIGHT = 40;
const int DEF_CONSOLE_SIZE = 15;
const int DEF_INFOBOX_WIDTH = (MAX_INFOBOX_WIDTH + MIN_INFOBOX_WIDTH) / 2;
const int DEF_ITEM_SIZE = 3;
const int DEF_ABILITY_SIZE = 4;
const int DEF_LIST_SIZE = 64;
const int DEF_LEVEL_EXP[MAX_LEVEL - 1] = { 50, 75, 100, 150, 200, 250, 400, 750, 1000 };
const int DEF_FRAME_WIDTH = MAX_NAME_LENGHT + 5; // " (..)"
const int DEF_FRAME_HEIGHT = 4;
const int DEF_CHARACTER_WIDTH = 36;
const int DEF_CHARACTER_HEIGHT = (DEF_CHARACTER_WIDTH * 2) / 3;

const double DOUBLE_PRECISION = 0.00001;

enum ArmorType {uArmorType, Helmet, Shoulders, Chest, Gloves, Legs, Feet};
enum WeaponType {uWeaponType, Axe, Staff};
//enum MonsterType {uMonsterType, Rat, Wolf, GiantSpider, Murloc, Witch, Dragon, ElwynnForestBoss};
enum Colour
{
	Black = 0, Blue = 1, Green = 2, Cyan = 3, Red = 4, Magenta = 5, Yellow = 6, LightGray = 7, DarkGray = 8,
	LightBlue = 9, LightGreen = 10, LightCyan = 11, LightRed = 12, LightMagenta = 13, LightYellow = 14, White = 15
};
