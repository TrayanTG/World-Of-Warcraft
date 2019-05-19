#pragma once

#include "HeadersAndConsts.h"
#include "Graphics.h"
#include "EnemyBox.h"
#include "FloatingTexts.h"
#include "PlayerClasses.h"

class Engine
{public: 
	static Engine s;
	bool cheatOn;
	
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD InputRecord[128];
	DWORD Events, currEvent;
	//COORD coord = { 0, 0 };
	CONSOLE_CURSOR_INFO cci;
	
	const COORD &getCoord();
	const WORD &getKey();
	void resetEvents();

	Engine();

	Player *myPlayer;
	char directory[MAX_PATH_LENGHT];

	void randomizeEnemies(EnemyBox **enemies, int enemyCnt);

public:
	void updateCursor();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	~Engine();
	
	static Engine& getInstance();

	void initMouse();
	void setCursorVisible(bool isVisible);

	void initHome(Box **boxes, int &cntBoxes, int &currBox);
	void initInventory(Box **boxes, Button &equipItem, Button &sellItem, int &cntBoxes, int &currBox, int &invBoxes, int &markedBox);
	void initShop(Box **boxes, Button &buyItem, int &cntBoxes, int &currBox, int &invBoxes, int &markedBox);
	void initAilityBook(Box **boxes, Button &eqSlot1, Button &eqSlot2, Button &eqSlot3, Button &eqSlot4, int &cntBoxes, int &currBox, int &invBoxes, int &markedBox);
	void loadMap(EnemyBox **enemies, int &enemyCnt);
	void enemyMove(EnemyBox **enemies, int &enemyCnt, int &currEnemy);

	void logIn();
	void Home();
	void Inventory();
	void Shop();
	void AbilityBook();
	void Map();
	void Play(Enemy enemy);
};