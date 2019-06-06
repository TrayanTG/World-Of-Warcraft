#pragma once

#include "HeadersAndConsts.h"
#include "Graphics.h"
#include "EnemyBox.h"
#include "FloatingTexts.h"
#include "PlayerClasses.h"

class Engine
{ 
	static bool cheatOn;
	
	static HANDLE hout;
	static HANDLE hin;
	static DWORD Events, currEvent;
	static CONSOLE_CURSOR_INFO cci;
	static INPUT_RECORD InputRecord[128];
	
	static COORD getCoord();
	static const WORD &getKey();
	static void resetEvents();

	Engine();

	static Player *myPlayer;
	static string directory;

	static void randomizeEnemies(EnemyBox **enemies, int enemyCnt);

public:
	static void inputReader();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	~Engine();

	static void initMouse();
	static void setCursorVisible(bool isVisible);

	static void initHome(Box **boxes, int &cntBoxes, int &currBox);
	static void initInventory(Box **boxes, Button &equipItem, Button &sellItem, int &cntBoxes, int &currBox, int &invBoxes, int &markedBox);
	static void initShop(Box **boxes, Button &buyItem, int &cntBoxes, int &currBox, int &invBoxes, int &markedBox);
	static void initAilityBook(Box **boxes, Button &eqSlot1, Button &eqSlot2, Button &eqSlot3, Button &eqSlot4, int &cntBoxes, int &currBox, int &invBoxes, int &markedBox);
	static void loadMap(EnemyBox **enemies, int &enemyCnt);
	static void enemyMove(EnemyBox **enemies, int &enemyCnt, int &currEnemy);

	static void logIn();
	static void Home();
	static void Inventory();
	static void Shop();
	static void AbilityBook();
	static void Map();
	static void Play(Enemy enemy);
};